#include "Win32/TeWin32GLSupport.h"
#include "TeGLTexture.h"
#include "Win32/TeWin32RenderWindow.h"
#include "TeGLRenderAPI.h"
#include "Win32/TeWin32Context.h"
#include "Win32/TeWin32VideoModeInfo.h"
#include "GL/wglew.h"
#include <algorithm>

GLenum __stdcall wglewContextInit (te::GLSupport* glSupport);

namespace te
{
    template<class C> void Remove_duplicates(C& c)
    {
        std::sort(c.begin(), c.end());
        typename C::iterator p = std::unique(c.begin(), c.end());
        c.erase(p, c.end());
    }

    Win32GLSupport::Win32GLSupport()
    {
        InitialiseWGL();
    }

    void Win32GLSupport::NotifyWindowCreated(Win32RenderWindow* window)
    {
        if (!_initialWindow)
            _initialWindow = window;
    }

    void Win32GLSupport::Start()
    {
    }

    void Win32GLSupport::Stop()
    {
        _initialWindow = nullptr;
    }

    void Win32GLSupport::InitializeExtensions()
    {
        assert(_initialWindow != nullptr);
        
        GLSupport::InitializeExtensions();

        wglewContextInit(this);

        // Check for W32 specific extensions probe function
        auto _wglGetExtensionsString = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsString");
        if(_wglGetExtensionsString == nullptr)
            return;

        const char* wglExtensions = _wglGetExtensionsString(_initialWindow->GetHDC());

        // Parse them, and add them to the main list
        StringStream ext;
        ext << wglExtensions;

        String instr;
        while (ext >> instr)
            _extensionList.push_back(instr);
    }

    SPtr<Win32Context> Win32GLSupport::CreateContext(HDC hdc, HGLRC externalGlrc)
    {
        GLRenderAPI* rapi = static_cast<GLRenderAPI*>(RenderAPI::InstancePtr());

        // If RenderAPI has initialized a context use that, otherwise we create our own
        HGLRC glrc = externalGlrc;
        bool createdNew = false;
        if (!rapi->IsContextInitialized())
        {
            if (externalGlrc == 0)
            {
                if (_hasAdvancedContext)
                {
                    int attribs[40];
                    int i = 0;

                    attribs[i++] = WGL_CONTEXT_MAJOR_VERSION_ARB;
                    attribs[i++] = 4;
                    attribs[i++] = WGL_CONTEXT_MINOR_VERSION_ARB;
                    attribs[i++] = 3;
                    attribs[i++] = WGL_CONTEXT_PROFILE_MASK_ARB;
                    attribs[i++] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;

#if (TE_DEBUG_MODE && (TE_OPENGL_4_3 || TE_OPENGLES_3_2))
                    attribs[i++] = WGL_CONTEXT_FLAGS_ARB;
                    attribs[i++] = WGL_CONTEXT_DEBUG_BIT_ARB;				
#endif

                    attribs[i++] = 0;
                    glrc = wglCreateContextAttribsARB(hdc, 0, attribs);
                }
                else
                    glrc = wglCreateContext(hdc);

                if (glrc == 0)
                    TE_ASSERT_ERROR(false, "wglCreateContext failed: " + TranslateWGLError());

                createdNew = true;
            }
        }
        else
        {
            glrc = wglGetCurrentContext();
        }

        return te_shared_ptr_new<Win32Context>(hdc, glrc, createdNew);
    }

    void* Win32GLSupport::GetProcAddress(const String& procname)
    {
        return (void*)wglGetProcAddress(procname.c_str());
    }

    void Win32GLSupport::InitialiseWGL()
    {
        // We need to create a dummy context in order to get functions that allow us to create a more advanced context.
        // It seems hacky but that's the only way to do it.
        
        LPCSTR dummyText = "Dummy";
#ifdef TE_STATIC_LIB
        HINSTANCE hinst = GetModuleHandle(NULL);
#else
        HINSTANCE hinst = GetModuleHandle("TeGLRenderAPI.dll");
#endif
        
        WNDCLASS dummyClass;
        memset(&dummyClass, 0, sizeof(WNDCLASS));
        dummyClass.style = CS_OWNDC;
        dummyClass.hInstance = hinst;
        dummyClass.lpfnWndProc = DummyWndProc;
        dummyClass.lpszClassName = dummyText;
        RegisterClass(&dummyClass);

        HWND hwnd = CreateWindow(
            dummyText,
            dummyText,
            WS_POPUP | WS_CLIPCHILDREN,
            0,
            0,
            32,
            32,
            0,
            0,
            hinst,
            0);

        if (hwnd == nullptr)
            TE_ASSERT_ERROR(false, "CreateWindow() failed");

        HDC hdc = GetDC(hwnd);

        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.cColorBits = 16;
        pfd.cDepthBits = 15;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        
        int format;
        if ((format = ChoosePixelFormat(hdc, &pfd)) != 0)
            SetPixelFormat(hdc, format, &pfd);

        HGLRC hrc = wglCreateContext(hdc);
        if (hrc)
        {
            HGLRC oldrc = wglGetCurrentContext();
            HDC oldhdc = wglGetCurrentDC();

            // If wglMakeCurrent fails, wglGetProcAddress will return null
            wglMakeCurrent(hdc, hrc);
            
            auto _wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
            auto _wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

            if (_wglCreateContextAttribsARB != nullptr)
            {
                WGLEW_GET_FUN(__wglewCreateContextAttribsARB) = _wglCreateContextAttribsARB;
                _hasAdvancedContext = true;
            }

            // Check for pixel format and multisampling support
            if (_wglGetExtensionsStringARB != nullptr)
            {
                std::istringstream wglexts(_wglGetExtensionsStringARB(hdc));
                String ext;
                while (wglexts >> ext)
                {
                    if (ext == "WGL_ARB_pixel_format")
                        _hasPixelFormatARB = true;
                    else if (ext == "WGL_ARB_multisample")
                        _hasMultisample = true;
                    else if (ext == "WGL_EXT_framebuffer_sRGB")
                        _hasHardwareGamma = true;
                }
            }

            if (_hasPixelFormatARB && _hasMultisample)
            {
                // Enumerate all formats w/ multisampling
                static const int iattr[] = {
                    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                    WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
                    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
                    WGL_SAMPLES_ARB, 2,
                    0
                };

                int formats[256];
                unsigned int count;
                WGLEW_GET_FUN(__wglewChoosePixelFormatARB) = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
                PFNWGLGETPIXELFORMATATTRIBIVARBPROC _wglGetPixelFormatAttribivARB =
                    (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
                if (WGLEW_GET_FUN(__wglewChoosePixelFormatARB)(hdc, iattr, 0, 256, formats, &count))
                {
                    // Determine what multisampling levels are offered
                    int query = WGL_SAMPLES_ARB, samples;
                    for (unsigned int i = 0; i < count; ++i)
                    {
                        if (_wglGetPixelFormatAttribivARB(hdc, formats[i], 0, 1, &query, &samples))
                        {
                            _multisampleLevels.push_back(samples);
                        }
                    }
                    Remove_duplicates(_multisampleLevels);
                }
            }

            wglMakeCurrent(oldhdc, oldrc);
            wglDeleteContext(hrc);
        }

        // Clean up our dummy window and class
        DestroyWindow(hwnd);
        UnregisterClass(dummyText, hinst);
    }

    LRESULT Win32GLSupport::DummyWndProc(HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp)
    {
        return DefWindowProc(hwnd, umsg, wp, lp);
    }

    bool Win32GLSupport::SelectPixelFormat(HDC hdc, int colorDepth, int multisample, bool hwGamma, bool depthStencil)
    {
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(pfd));
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = (colorDepth > 16)? 24 : static_cast<BYTE>(colorDepth);
        pfd.cAlphaBits = (colorDepth > 16)? 8 : 0;

        if (depthStencil)
        {
            pfd.cDepthBits = 24;
            pfd.cStencilBits = 8;
        }

        int format = 0;

        int useHwGamma = hwGamma? GL_TRUE : GL_FALSE;

        if (multisample && (!_hasMultisample || !_hasPixelFormatARB))
            return false;

        if (hwGamma && !_hasHardwareGamma)
            return false;
        
        if ((multisample || hwGamma) && WGLEW_GET_FUN(__wglewChoosePixelFormatARB))
        {
            // Use WGL to test extended caps (multisample, sRGB)
            Vector<int> attribList;
            attribList.push_back(WGL_DRAW_TO_WINDOW_ARB); attribList.push_back(GL_TRUE);
            attribList.push_back(WGL_SUPPORT_OPENGL_ARB); attribList.push_back(GL_TRUE);
            attribList.push_back(WGL_DOUBLE_BUFFER_ARB); attribList.push_back(GL_TRUE);
            attribList.push_back(WGL_SAMPLE_BUFFERS_ARB); attribList.push_back(GL_TRUE);
            attribList.push_back(WGL_ACCELERATION_ARB); attribList.push_back(WGL_FULL_ACCELERATION_ARB);
            attribList.push_back(WGL_COLOR_BITS_ARB); attribList.push_back(pfd.cColorBits);
            attribList.push_back(WGL_ALPHA_BITS_ARB); attribList.push_back(pfd.cAlphaBits);
            attribList.push_back(WGL_DEPTH_BITS_ARB); attribList.push_back(pfd.cDepthBits);
            attribList.push_back(WGL_STENCIL_BITS_ARB); attribList.push_back(pfd.cStencilBits);
            attribList.push_back(WGL_SAMPLES_ARB); attribList.push_back(multisample);

            if (useHwGamma && CheckExtension("WGL_EXT_framebuffer_sRGB"))
            {
                attribList.push_back(WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT);
                attribList.push_back(GL_TRUE);
            }

            attribList.push_back(0); // Terminator

            UINT numFormats;
            if (!WGLEW_GET_FUN(__wglewChoosePixelFormatARB)(hdc, &(attribList[0]), NULL, 1, &format, &numFormats) || numFormats <= 0)
                return false;
        }
        else
        {
            format = ChoosePixelFormat(hdc, &pfd);
        }

        return format && SetPixelFormat(hdc, format, &pfd);
    }

    SPtr<VideoModeInfo> Win32GLSupport::GetVideoModeInfo() const
    {
        return te_shared_ptr_new<Win32VideoModeInfo>();
    }

    String TranslateWGLError()
    {
        int winError = GetLastError();
        char errDesc[255];

        FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            winError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) errDesc,
            255,
            NULL);

        return String(errDesc);
    }
}
