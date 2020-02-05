#pragma once

#include "TeCorePrerequisites.h"
#include "CoreUtility/TeCoreObject.h"
#include "RenderAPI/TeBlendState.h"
#include "RenderAPI/TeRasterizerState.h"
#include "RenderAPI/TeDepthStencilState.h"
#include "RenderAPI/TeGpuProgram.h"
#include "RenderAPI/TeGpuPipelineState.h"

namespace te
{
    /** Descriptor structure used for initializing a shader pass. */
	struct PASS_DESC
	{
		BLEND_STATE_DESC BlendStateDesc;
		RASTERIZER_STATE_DESC RasterizerStateDesc;
		DEPTH_STENCIL_STATE_DESC DepthStencilStateDesc;
		UINT32 StencilRefValue;

		GPU_PROGRAM_DESC VertexProgramDesc;
		GPU_PROGRAM_DESC PixelProgramDesc;
		GPU_PROGRAM_DESC GeometryProgramDesc;
		GPU_PROGRAM_DESC HullProgramDesc;
		GPU_PROGRAM_DESC DomainProgramDesc;
	};

    class TE_CORE_EXPORT Pass : public CoreObject
    {
    public:
        virtual ~Pass();

        /**	Returns true if this pass has some element of transparency. */
        bool HasBlending() const;

        /** Gets the stencil reference value that is used when performing operations using the stencil buffer. */
        UINT32 GetStencilRefValue() const { return _data.StencilRefValue; }

        /** Returns the GPU program descriptor for the specified GPU program type. */
        const GPU_PROGRAM_DESC& GetProgramDesc(GpuProgramType type) const;

        /**
         * Returns the graphics pipeline state describing this pass, or null if its a compute pass.
         * Only valid after compile() has been called.
         */
        const SPtr<GraphicsPipelineState>& GetGraphicsPipelineState() const { return _graphicsPipelineState; }

        /** Creates either the graphics or the compute pipeline state from the stored pass data. */
        void CreatePipelineState();

        /**
         * Initializes the pass internals by compiling the GPU programs and creating the relevant pipeline state. This
         * method must be called before pass pipelines can be retrieved. After initial compilation further calls do this
         * method will perform no operation.
         */
        void Compile();

        /**	Creates a new empty pass. */
        static SPtr<Pass> Create(const PASS_DESC& desc);

        /**	Creates a new empty pass but doesn't initialize it. */
        static SPtr<Pass> CreateEmpty();

        /** @copydoc CoreObject::FrameSync */
        void FrameSync() override;

    protected:
        Pass();
        Pass(const PASS_DESC & desc);

    protected:
        PASS_DESC _data;
		SPtr<GraphicsPipelineState> _graphicsPipelineState;
    };
}
