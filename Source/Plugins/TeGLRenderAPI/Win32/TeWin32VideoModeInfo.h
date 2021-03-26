#pragma once

#include "TeGLRenderAPIPrerequisites.h"
#include "Renderapi/TeVideoMode.h"

namespace te
{
    /** @copydoc VideoMode */
    class Win32VideoMode : public VideoMode
    {
    public:
        Win32VideoMode(UINT32 width, UINT32 height, float refreshRate, UINT32 outputIdx);

    private:
        friend class Win32VideoOutputInfo;
    };

    /** @copydoc VideoOutputInfo */
    class Win32VideoOutputInfo : public VideoOutputInfo
    {
    public:
        Win32VideoOutputInfo(HMONITOR monitorHandle, UINT32 outputIdx);

        /**	Gets a Win32 handle to the monitor referenced by this object. */
        HMONITOR GetMonitorHandle() const { return _monitorHandle; }

    private:
        HMONITOR _monitorHandle;
    };

    /** @copydoc VideoModeInfo */
    class Win32VideoModeInfo : public VideoModeInfo
    {
    public:
        Win32VideoModeInfo();
    };
}
