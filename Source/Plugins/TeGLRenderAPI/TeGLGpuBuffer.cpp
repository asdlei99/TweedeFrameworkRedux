#include "TeGLGpuBuffer.h"
#include "TeGLRenderAPI.h"
#include "TeGLPixelFormat.h"
#include "TeGLHardwareBuffer.h"

namespace te
{
    static void DeleteBuffer(HardwareBuffer* buffer)
    {
        te_pool_delete(static_cast<GLHardwareBuffer*>(buffer));
    }

    GLGpuBuffer::GLGpuBuffer(const GPU_BUFFER_DESC& desc, GpuDeviceFlags deviceMask)
        : GpuBuffer(desc, deviceMask)
    {
        assert((deviceMask == GDF_DEFAULT || deviceMask == GDF_PRIMARY) && "Multiple GPUs not supported natively on OpenGL.");
        _format = GLPixelUtil::GetBufferFormat(desc.Format);
    }

    GLGpuBuffer::GLGpuBuffer(const GPU_BUFFER_DESC& desc, SPtr<HardwareBuffer> underlyingBuffer)
        : GpuBuffer(desc, std::move(underlyingBuffer))
    { 
        _format = GLPixelUtil::GetBufferFormat(desc.Format);
    }

    GLGpuBuffer::~GLGpuBuffer()
    {
        if (_properties.GetType() != GBT_STRUCTURED)
        {
            glDeleteTextures(1, &_textureID);
            TE_CHECK_GL_ERROR();
        }
    }

    void GLGpuBuffer::Initialize()
    {
        _bufferDeleter = &DeleteBuffer;

        // Create a new buffer if not wrapping an external one
        if (!_buffer)
        {
            if (_properties.GetType() == GBT_STRUCTURED)
            {
#if TE_OPENGL_4_2 || TE_OPENGLES_3_1
                const auto& props = GetProperties();
                UINT32 size = props.GetElementCount() * props.GetElementSize();
                _buffer = te_pool_new<GLHardwareBuffer>(GL_SHADER_STORAGE_BUFFER, size, props.GetUsage());
#else
                TE_DEBUG("SSBOs are not supported on the current OpenGL version.");
#endif
            }
            else
            {
                const auto& props = GetProperties();
                UINT32 size = props.GetElementCount() * props.GetElementSize();
                _buffer = te_pool_new<GLHardwareBuffer>(GL_TEXTURE_BUFFER, size, props.GetUsage());
            }
        }

        if (_properties.GetType() != GBT_STRUCTURED)
        {
            // Create texture with a specific format
            glGenTextures(1, &_textureID);
            TE_CHECK_GL_ERROR();

            glBindTexture(GL_TEXTURE_BUFFER, _textureID);
            TE_CHECK_GL_ERROR();

            glTexBuffer(GL_TEXTURE_BUFFER, _format, static_cast<GLHardwareBuffer*>(_buffer)->GetGLBufferId());
            TE_CHECK_GL_ERROR();
        }

        GpuBuffer::Initialize();
    }
}
