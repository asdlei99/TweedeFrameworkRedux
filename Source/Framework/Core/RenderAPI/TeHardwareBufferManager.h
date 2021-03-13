#pragma once

#include "TeCorePrerequisites.h"
#include "Utility/TeModule.h"
#include "RenderAPI/TeVertexBuffer.h"
#include "RenderAPI/TeIndexBuffer.h"
#include "RenderAPI/TeVertexDeclaration.h"
#include "RenderAPI/TeGpuParams.h"

namespace te
{
    /** Handles creation of various hardware buffers. */
    class TE_CORE_EXPORT HardwareBufferManager : public Module<HardwareBufferManager>
    {
    public:
        HardwareBufferManager() = default;
        virtual ~HardwareBufferManager() = default;

        TE_MODULE_STATIC_HEADER_MEMBER(HardwareBufferManager)

        /**
         * Creates a new vertex buffer used for holding number of vertices and other per-vertex data. Buffer can be bound
         * to the pipeline and its data can be passed to the active vertex GPU program.
         *
         * @param[in]	desc	Description of the buffer to create.
         */
        SPtr<VertexBuffer> CreateVertexBuffer(const VERTEX_BUFFER_DESC& desc, GpuDeviceFlags deviceMask = GDF_DEFAULT);

        /**
         * Creates a new index buffer that holds indices referencing vertices in a vertex buffer. Indices are interpreted
         * by the pipeline and vertices are drawn in the order specified in the index buffer.
         *
         * @param[in]	desc	Description of the buffer to create.
         */
        SPtr<IndexBuffer> CreateIndexBuffer(const INDEX_BUFFER_DESC& desc, GpuDeviceFlags deviceMask = GDF_DEFAULT);

        /**
         * Creates a new vertex declaration from a list of vertex elements.
         *
         * @param[in]	desc	Description of the object to create.
         */
        SPtr<VertexDeclaration> CreateVertexDeclaration(const SPtr<VertexDataDesc>& desc, GpuDeviceFlags deviceMask = GDF_DEFAULT);

        /**
         * Creates a new vertex declaration from a list of vertex elements.
         *
         * @param[in]	elements		List of elements to initialize the declaration with.
         * @param[in]	deviceMask		Mask that determines on which GPU devices should the object be created on.
         */
        SPtr<VertexDeclaration> CreateVertexDeclaration(const Vector<VertexElement>& elements,
            GpuDeviceFlags deviceMask = GDF_DEFAULT);

        /**
         * @copydoc te::HardwareBufferManager::CreateGpuParamBlockBuffer
         * @param[in]	deviceMask		Mask that determines on which GPU devices should the object be created on.
         */
        SPtr<GpuParamBlockBuffer> CreateGpuParamBlockBuffer(UINT32 size,
            GpuBufferUsage usage = GBU_DYNAMIC, GpuDeviceFlags deviceMask = GDF_DEFAULT);

        /**
         * @copydoc te::HardwareBufferManager::createGpuBuffer
         * @param[in]	deviceMask		Mask that determines on which GPU devices should the object be created on.
         */
        SPtr<GpuBuffer> CreateGpuBuffer(const GPU_BUFFER_DESC& desc, GpuDeviceFlags deviceMask = GDF_DEFAULT);

        /** @copydoc GpuBuffer::Create(const GPU_BUFFER_DESC&, SPtr<HardwareBuffer>) */
        SPtr<GpuBuffer> CreateGpuBuffer(const GPU_BUFFER_DESC& desc, SPtr<HardwareBuffer> underlyingBuffer);

        /** @copydoc GpuParams::create(const SPtr<GpuPipelineParamInfo>&, GpuDeviceFlags) */
        SPtr<GpuParams> CreateGpuParams(const SPtr<GpuPipelineParamInfo>& paramInfo,
            GpuDeviceFlags deviceMask = GDF_DEFAULT);

    protected:
        friend class IndexBuffer;
        friend class VertexBuffer;
        friend class VertexDeclaration;
        friend class GpuParamBlockBuffer;
        friend class GpuBuffer;

        /** Key for use in the vertex declaration map. */
        struct VertexDeclarationKey
        {
            VertexDeclarationKey(const Vector<VertexElement>& elements);

            class HashFunction
            {
            public:
                size_t operator()(const VertexDeclarationKey& key) const;
            };

            class EqualFunction
            {
            public:
                bool operator()(const VertexDeclarationKey& lhs, const VertexDeclarationKey& rhs) const;
            };

            Vector<VertexElement> elements;
        };

        /** @copydoc createVertexBuffer */
        virtual SPtr<VertexBuffer> CreateVertexBufferInternal(const VERTEX_BUFFER_DESC& desc,
            GpuDeviceFlags deviceMask = GDF_DEFAULT) = 0;

        /** @copydoc createIndexBuffer */
        virtual SPtr<IndexBuffer> CreateIndexBufferInternal(const INDEX_BUFFER_DESC& desc,
            GpuDeviceFlags deviceMask = GDF_DEFAULT) = 0;

        /** @copydoc createVertexDeclaration(const Vector<VertexElement>&, GpuDeviceFlags) */
        virtual SPtr<VertexDeclaration> createVertexDeclarationInternal(const Vector<VertexElement>& elements,
            GpuDeviceFlags deviceMask = GDF_DEFAULT);

        /** @copydoc CreateGpuParamBlockBuffer */
        virtual SPtr<GpuParamBlockBuffer> CreateGpuParamBlockBufferInternal(UINT32 size,
            GpuBufferUsage usage = GBU_DYNAMIC, GpuDeviceFlags deviceMask = GDF_DEFAULT) = 0;

        /** @copydoc CreateGpuBuffer(const GPU_BUFFER_DESC&, GpuDeviceFlags) */
        virtual SPtr<GpuBuffer> CreateGpuBufferInternal(const GPU_BUFFER_DESC& desc,
            GpuDeviceFlags deviceMask = GDF_DEFAULT) = 0;

        /** @copydoc CreateGpuBuffer(const GPU_BUFFER_DESC&, SPtr<HardwareBuffer>) */
        virtual SPtr<GpuBuffer> CreateGpuBufferInternal(const GPU_BUFFER_DESC& desc,
            SPtr<HardwareBuffer> underlyingBuffer) = 0;

        /** @copydoc CreateGpuParams */
        virtual SPtr<GpuParams> CreateGpuParamsInternal(const SPtr<GpuPipelineParamInfo>& paramInfo,
            GpuDeviceFlags deviceMask = GDF_DEFAULT);

        typedef UnorderedMap<VertexDeclarationKey, SPtr<VertexDeclaration>,
            VertexDeclarationKey::HashFunction, VertexDeclarationKey::EqualFunction> DeclarationMap;
    
    protected:
        DeclarationMap _cachedDeclarations;
    };
}