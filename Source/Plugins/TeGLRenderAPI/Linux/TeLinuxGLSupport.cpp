#include "Private/Linux/TeLinuxPlatform.h"
#include "Linux/TeLinuxGLSupport.h"
#include "Linux/TeLinuxContext.h"
#include "Linux/TeLinuxRenderWindow.h"
#include "Linux/TeLinuxVideoModeInfo.h"
#include "TeGLRenderAPI.h"

namespace te
{
    bool extGLX_ARB_multisample = false;
    bool extGLX_ARB_framebuffer_sRGB = false;
    bool extGLX_EXT_framebuffer_sRGB = false;
    bool extGLX_ARB_create_context = false;
    bool extGLX_ARB_create_context_profile = false;
    bool extGLX_EXT_swap_control = false;
    bool extGLX_MESA_swap_control = false;
    bool extGLX_SGI_swap_control = false;

    typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = nullptr;

    bool Load_ARB_create_context()
    {
        glXCreateContextAttribsARB =
                (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

        return glXCreateContextAttribsARB != nullptr;
    }

    glXSwapIntervalEXTProc glXSwapIntervalEXT = nullptr;
    glXSwapIntervalMESAProc glXSwapIntervalMESA = nullptr;
    glXSwapIntervalSGIProc glXSwapIntervalSGI = nullptr;

    glXChooseFBConfigProc glXChooseFBConfig = nullptr;
    glXGetFBConfigAttribProc glXGetFBConfigAttrib = nullptr;
    glXGetVisualFromFBConfigProc glXGetVisualFromFBConfig = nullptr;

    bool Load_EXT_swap_control()
    {
        glXSwapIntervalEXT = (glXSwapIntervalEXTProc)glXGetProcAddressARB((const GLubyte*)"glXSwapIntervalEXT");

        return glXSwapIntervalEXT != nullptr;
    }

    bool Load_MESA_swap_control()
    {
        glXSwapIntervalMESA = (glXSwapIntervalMESAProc)glXGetProcAddressARB((const GLubyte*)"glXSwapIntervalMESA");

        return glXSwapIntervalMESA != nullptr;
    }

    bool Load_SGI_swap_control()
    {
        glXSwapIntervalSGI = (glXSwapIntervalSGIProc)glXGetProcAddressARB((const GLubyte*)"glXSwapIntervalSGI");

        return glXSwapIntervalSGI != nullptr;
    }

    typedef bool (*ExtensionFunc)(void);

    struct GLExtension
    {
        const char* name;
        bool* status;
        ExtensionFunc func;
    };

    static GLExtension gExtensionMap[] = {
        { "GLX_ARB_multisample", &extGLX_ARB_multisample, nullptr },
        { "GLX_ARB_framebuffer_sRGB", &extGLX_ARB_framebuffer_sRGB, nullptr },
        { "GLX_EXT_framebuffer_sRGB", &extGLX_EXT_framebuffer_sRGB, nullptr },
        { "GLX_ARB_create_context", &extGLX_ARB_create_context, Load_ARB_create_context },
        { "GLX_ARB_create_context_profile", &extGLX_ARB_create_context_profile, nullptr },
        { "GLX_EXT_swap_control", &extGLX_EXT_swap_control, Load_EXT_swap_control },
        { "GLX_MESA_swap_control", &extGLX_MESA_swap_control, Load_MESA_swap_control },
        { "GLX_SGI_swap_control", &extGLX_SGI_swap_control, Load_SGI_swap_control },
    };

    LinuxGLSupport::LinuxGLSupport()
    { }

    void LinuxGLSupport::Start()
    {
        // Retrieve all essential extensions
        LinuxPlatform::LockX();

        ::Display* display = LinuxPlatform::GetXDisplay();
        const char* glExtensions = glXQueryExtensionsString(display, DefaultScreen(display));

        const char* iter = glExtensions;
        do
        {
            const char* start = iter;
            while(*iter != ' ' && *iter)
                iter++;

            const char* end = iter;
            std::string name(start, end);

            UINT32 numExtensions = sizeof(gExtensionMap) / sizeof(gExtensionMap[0]);
            for (UINT32 i = 0; i < numExtensions; ++i)
            {
                if(strcmp(name.c_str(), gExtensionMap[i].name) == 0)
                {
                    if(gExtensionMap[i].status != nullptr)
                    {
                        if (gExtensionMap[i].func != nullptr)
                            *gExtensionMap[i].status = gExtensionMap[i].func();
                        else
                            *gExtensionMap[i].status = true;
                    }
                    else
                    {
                        if (gExtensionMap[i].func != nullptr)
                            gExtensionMap[i].func();
                    }
                }

            }

        } while(*iter++);

        glXChooseFBConfig = (glXChooseFBConfigProc)glXGetProcAddressARB((const GLubyte*)"glXChooseFBConfig");
        glXGetFBConfigAttrib = (glXGetFBConfigAttribProc)glXGetProcAddressARB((const GLubyte*)"glXGetFBConfigAttrib");
        glXGetVisualFromFBConfig = (glXGetVisualFromFBConfigProc)glXGetProcAddressARB((const GLubyte*)"glXGetVisualFromFBConfig");

        LinuxPlatform::UnlockX();
    }

    void LinuxGLSupport::Stop()
    {
        // Do nothing
    }

    SPtr<LinuxContext> LinuxGLSupport::CreateContext(::Display* x11display, XVisualInfo& visualInfo)
    {
        GLRenderAPI* rapi = static_cast<GLRenderAPI*>(RenderAPI::InstancePtr());

        // If RenderAPI has initialized a context use that, otherwise we create our own
        if (!rapi->IsContextInitialized())
            return te_shared_ptr_new<LinuxContext>(x11display, visualInfo);
        else
        {
            SPtr<GLContext> context = rapi->GetMainContext();
            return std::static_pointer_cast<LinuxContext>(context);
        }
    }

    void* LinuxGLSupport::GetProcAddress(const String& procname)
    {
        return (void*)glXGetProcAddressARB((const GLubyte*)procname.c_str());
    }

    GLVisualConfig LinuxGLSupport::FindBestVisual(::Display* display, bool depthStencil, UINT32 multisample, bool srgb) const
    {
        INT32 VISUAL_ATTRIBS[] =
        {
            GLX_X_RENDERABLE,       True,
            GLX_DRAWABLE_TYPE,      GLX_WINDOW_BIT,
            GLX_RENDER_TYPE,        GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE,      GLX_TRUE_COLOR,
            GLX_RED_SIZE,           8,
            GLX_GREEN_SIZE,         8,
            GLX_BLUE_SIZE,          8,
            GLX_ALPHA_SIZE,         8,
            GLX_DOUBLEBUFFER,       True,
            GLX_DEPTH_SIZE,         depthStencil ? 24 : 0,
            GLX_STENCIL_SIZE,       depthStencil ? 8 : 0,
            GLX_SAMPLE_BUFFERS,     multisample > 1 ? 1 : 0,
            0
        };

        INT32 numConfigs;
        GLXFBConfig* configs = glXChooseFBConfig(display, DefaultScreen(display), VISUAL_ATTRIBS, &numConfigs);
        GLVisualCapabilities* caps = te_allocate<GLVisualCapabilities>(numConfigs * sizeof(GLVisualCapabilities));

        // Find a config that best matches the requested properties
        INT32 bestScore = 0;
        INT32 bestConfig = -1;
        for (int i = 0; i < numConfigs; ++i)
        {
            INT32 configScore = 0;

            // Depth buffer contributes the most to score
            INT32 depth, stencil;
            glXGetFBConfigAttrib(display, configs[i], GLX_DEPTH_SIZE, &depth);
            glXGetFBConfigAttrib(display, configs[i], GLX_STENCIL_SIZE, &stencil);

            // Depth buffer was requested
            if(depthStencil)
            {
                INT32 score = 0;
                if (depth == 24 && stencil == 8)
                    score = 10000;
                else if (depth == 32 && stencil == 8)
                    score = 9000;
                else if (depth == 32)
                    score = 8000;
                else if (depth == 24)
                    score = 7000;
                else if (depth == 16)
                    score = 6000;

                if (score > 0)
                {
                    configScore += score;
                    caps[i].depthStencil = true;
                }
            }
            else // Depth buffer not requested, prefer configs without it
            {
                if(depth == 0 && stencil == 0)
                    configScore += 10000;
            }

            // sRGB contributes second most
            if(srgb)
            {
                INT32 hasSRGB = 0;

                if(extGLX_EXT_framebuffer_sRGB)
                    glXGetFBConfigAttrib(display, configs[i], GLX_FRAMEBUFFER_SRGB_CAPABLE_EXT, &hasSRGB);

                if(!hasSRGB && extGLX_ARB_framebuffer_sRGB)
                    glXGetFBConfigAttrib(display, configs[i], GLX_FRAMEBUFFER_SRGB_CAPABLE_EXT, &hasSRGB);

                if(hasSRGB)
                {
                    configScore += 500;
                    caps[i].srgb = true;
                }
            }

            if((multisample >= 1) && extGLX_ARB_multisample)
            {
                INT32 hasMultisample, numSamples;
                glXGetFBConfigAttrib(display, configs[i], GLX_SAMPLE_BUFFERS, &hasMultisample);
                glXGetFBConfigAttrib(display, configs[i], GLX_SAMPLES, &numSamples);

                if(hasMultisample && (numSamples <= (INT32)multisample))
                {
                    configScore += (32 - (multisample - numSamples)) * 10;
                    caps[i].numSamples = (UINT32)numSamples;
                }
            }

            if(configScore > bestScore)
            {
                bestScore = configScore;
                bestConfig = i;
            }
        }

        GLVisualConfig output;

        if(bestConfig == -1)
        {
            if(numConfigs > 0)
                bestConfig = 0;
            else
            {
                // Something went wrong
                XFree(configs);
                te_delete(caps);

                return output;
            }
        }

        XVisualInfo* visualInfo = glXGetVisualFromFBConfig(display, configs[bestConfig]);

        output.visualInfo = *visualInfo;
        output.caps = caps[bestConfig];

        XFree(configs);
        XFree(visualInfo);
        te_delete(caps);

        return output;
    }

    SPtr<VideoModeInfo> LinuxGLSupport::GetVideoModeInfo() const
    {
        return te_shared_ptr_new<LinuxVideoModeInfo>();
    }
}
