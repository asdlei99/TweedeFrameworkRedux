#include "Linux/TeLinuxVideoModeInfo.h"
#include "Private/Linux/TeLinuxPlatform.h"
#include <X11/extensions/Xrandr.h>

#define XRANDR_ROTATION_LEFT    (1 << 1)
#define XRANDR_ROTATION_RIGHT   (1 << 3)

namespace te
{
    LinuxVideoModeInfo::LinuxVideoModeInfo()
    {
        LinuxPlatform::LockX();

        ::Display* display = LinuxPlatform::GetXDisplay();

        INT32 minor, major;
        XRRQueryVersion(display, &minor, &major);

        INT32 defaultScreen = XDefaultScreen(display);
        RROutput primaryOutput = XRRGetOutputPrimary(display, RootWindow(display, defaultScreen));

        INT32 screenCount = XScreenCount(display);
        for(INT32 i = 0; i < screenCount; i++)
        {
            XRRScreenResources* screenRes = XRRGetScreenResources(display, RootWindow(display, i));

            for(INT32 j = 0; j < screenRes->noutput; j++)
            {
                XRROutputInfo* outputInfo = XRRGetOutputInfo(display, screenRes, screenRes->outputs[j]);

                if(outputInfo == nullptr || outputInfo->crtc == 0 || outputInfo->connection == RR_Disconnected)
                {
                    XRRFreeOutputInfo(outputInfo);
                    continue;
                }

                XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(display, screenRes, outputInfo->crtc);
                if(crtcInfo == nullptr)
                {
                    XRRFreeCrtcInfo(crtcInfo);
                    XRRFreeOutputInfo(outputInfo);
                    continue;
                }

                VideoOutputInfo* output = te_new<LinuxVideoOutputInfo>(display, i, outputInfo, crtcInfo, screenRes,
                        screenRes->outputs[j], (UINT32)_outputs.size());

                // Make sure the primary output is the first in the output list
                if(i == defaultScreen && screenRes->outputs[j] == primaryOutput)
                    _outputs.insert(_outputs.begin(), output);
                else
                    _outputs.push_back(output);

                XRRFreeCrtcInfo(crtcInfo);
                XRRFreeOutputInfo(outputInfo);
            }

            XRRFreeScreenResources(screenRes);
        }

        LinuxPlatform::UnlockX();
    }

    LinuxVideoOutputInfo::LinuxVideoOutputInfo(::Display* x11Display, INT32 screen, XRROutputInfo* outputInfo,
        XRRCrtcInfo* crtcInfo, XRRScreenResources* screenRes, RROutput outputID, UINT32 outputIdx)
            : _outputID(outputID)
            , _screen(screen)
    {
        RRMode currentMode = crtcInfo->mode;

        // Parse output name
        Atom EDID = XInternAtom(x11Display, "EDID", False);

        INT32 numOutputProps;
        Atom* outputProps = XRRListOutputProperties(x11Display, _outputID, &numOutputProps);

        for(INT32 k = 0; k < numOutputProps; k++)
        {
            if(outputProps[k] != EDID)
                continue;

            Atom actualType;
            unsigned long numItems, bytesAfter;
            INT32 actualFormat;
            UINT8* data;

            Status status = XRRGetOutputProperty(x11Display, _outputID, outputProps[k], 0, 128, False,
                    False, AnyPropertyType, &actualType, &actualFormat, &numItems, &bytesAfter, &data);
            if(status == Success)
            {
                // Decode EDID to get the name
                for(UINT32 l = 0; l < 4; l++)
                {
                    INT32 idx = 0x36 + l * 18;
                    if(data[idx] == 0 && data[idx + 1] == 0 && data[idx + 3] == 0xFC)
                    {
                        UINT8* nameSrc = &data[idx + 5];

                        char name[14];
                        for(UINT32 m = 0; m < 13; m++)
                        {
                            if(nameSrc[m] == 0x0a)
                            {
                                name[m] = '\0';
                                break;
                            }
                            else if(nameSrc[m] == 0x00)
                                name[m] = ' ';
                            else
                                name[m] = nameSrc[m];
                        }

                        name[13] = '\0';
                        _name = String(name);
                    }
                }

                continue;
            }

            XFree(data);
            break;
        }

        XFree(outputProps);

        // Use the output name if display name cannot be found
        if(_name.empty())
            _name = outputInfo->name;

        // Enumerate all valid resolutions
        for(INT32 k = 0; k < screenRes->nmode; k++)
        {
            const XRRModeInfo& modeInfo = screenRes->modes[k];

            UINT32 width, height;

            if(crtcInfo->rotation & (XRANDR_ROTATION_LEFT | XRANDR_ROTATION_RIGHT))
            {
                width = modeInfo.height;
                height = modeInfo.width;
            }
            else
            {
                width = modeInfo.width;
                height = modeInfo.height;
            }

            float refreshRate;
            if(modeInfo.hTotal != 0 && modeInfo.vTotal != 0)
                refreshRate = (float)(modeInfo.dotClock / (double)(modeInfo.hTotal * modeInfo.vTotal));
            else
                refreshRate = 0.0f;

            LinuxVideoMode* videoMode = new (te_allocate<LinuxVideoMode>())
                    LinuxVideoMode(width, height, refreshRate, outputIdx, modeInfo.id);
            _videoModes.push_back(videoMode);
        }

        // Save current desktop mode
        for(INT32 k = 0; k < screenRes->nmode; k++)
        {
            if(screenRes->modes[k].id == currentMode)
            {
                _desktopVideoMode = new (te_allocate<LinuxVideoMode>())
                        LinuxVideoMode(_videoModes[k]->_width, _videoModes[k]->_height,
                        _videoModes[k]->_refreshRate, _videoModes[k]->_outputIdx, currentMode);
                break;
            }
        }
    }

    LinuxVideoMode::LinuxVideoMode(UINT32 width, UINT32 height, float refreshRate, UINT32 outputIdx)
        : VideoMode(width, height, refreshRate, outputIdx)
        , _modeID((RRMode)-1)
    { }

    LinuxVideoMode::LinuxVideoMode(UINT32 width, UINT32 height, float refreshRate, UINT32 outputIdx, RRMode modeID)
        : VideoMode(width, height, refreshRate, outputIdx)
        , _modeID(modeID)
    { }
}
