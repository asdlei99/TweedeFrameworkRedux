#pragma once

#include "TeCorePrerequisites.h"
#include "Renderer/TeRendererMaterial.h"
#include "Renderer/TeParamBlocks.h"

namespace te
{
    TE_PARAM_BLOCK_BEGIN(ToneMappingParamDef)
        TE_PARAM_BLOCK_ENTRY(float, gGamma)
        TE_PARAM_BLOCK_ENTRY(float, gExposure)
        TE_PARAM_BLOCK_ENTRY(float, gContrast)
        TE_PARAM_BLOCK_ENTRY(float, gBrightness)
        TE_PARAM_BLOCK_ENTRY(INT32, gMSAACount)
    TE_PARAM_BLOCK_END

    extern ToneMappingParamDef gToneMappingParamDef;

    /** Shader that performs Fast Approximate anti-aliasing. */
    class ToneMappingMat : public RendererMaterial<ToneMappingMat>
    {
        RMAT_DEF(BuiltinShader::ToneMapping);

    public:
        ToneMappingMat();

        /**
         * Renders the post-process effect with the provided parameters.
         *
         * @param[in]	source		Input texture to apply ToneMappingMat to.
         * @param[in]	destination	Output target to which to write the antialiased image to.
         */
        void Execute(const SPtr<Texture>& source, const SPtr<RenderTarget>& destination, 
            INT32 MSAACount = 1, float gamma = 2.2f, float exposure = 1.0f, float contrast = 1.0f, float brightness = 0.0f);

    private:
        SPtr<GpuParamBlockBuffer> _paramBuffer;
    };
}
