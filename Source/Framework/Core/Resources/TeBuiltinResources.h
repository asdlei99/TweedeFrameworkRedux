#pragma once

#include "TeCorePrerequisites.h"
#include "Utility/TeModule.h"
#include "RenderAPI/TeBlendState.h"
#include "RenderAPI/TeRasterizerState.h"
#include "RenderAPI/TeDepthStencilState.h"
#include "RenderAPI/TeSamplerState.h"
#include "Math/TeMath.h"
#include "Math/TeMatrix4.h"
#include "Math/TeVector2.h"
#include "RenderAPI/TeGpuProgram.h"
#include "Material/TeShader.h"

namespace te
{
    /** Types of builtin shaders that are always available. */
    enum class BuiltinShader
    {
        /** Shader used for opaque 3D geometry. */
        Opaque = 0x0,
        /** Shader used for transparent 3D geometry. */
        Transparent = 0x1,
        /** Shader used for transparent 3D geometry without backface culling. */
        TransparentCullNone = 0x2,
        /** Shader used to blit a texture on a screen quad */
        Blit = 0x3,
        /** Shader used to render a skybox in a scene */
        Skybox = 0x4,
        /** Shader used for FXAA post process */
        FXAA = 0x5,
        /** Shader used for tone mapping post process */
        ToneMapping = 0x6,
        /** Shader used for bloom post process */
        Bloom = 0x7,
        /** Shader used for motion blur post process */
        MotionBlur = 0x8,
        /** Shader used for gaussian blur post process */
        GaussianBlur = 0x9,
        /** Shader used for picking */
        Picking = 0xA,
        /** Shader used for hud picking */
        HudPicking = 0xB,
        /** Shader used for selection */
        Selection = 0xC,
        /** Shader used for hud draw and selection */
        HudSelection = 0xD,
        /** Shared used for Bullet physic debug display */
        BulletDebug = 0xE,
        /** Shader used for ssao */
        SSAO = 0xF,
        /** Shader used to blur ssao output */
        SSAOBlur = 0x10,
        /** Shader used to downswample ssao input (depth and normal buffer) */
        SSAODownSample = 0x11,
        /** Shader used to render decals on top of rendered objects */
        Decal = 0x12,
        /** Shader used for material's preview for opaque objects */
        PreviewOpaque = 0x13,
        /** Shader used for material's preview for transparent objects */
        PreviewTransparent = 0x14,
    };

    /** Types of builtin shaders that are always available. */
    enum class BuiltinSampler
    {
        /** Anistropic sampler. */
        Anisotropic,
        /** Bilinear sampler. */
        Bilinear,
        /** Trilinear sampler. */
        Trilinear
    };

    /**	Holds references to built-in resources used by the core engine. */
    class TE_CORE_EXPORT BuiltinResources : public Module<BuiltinResources>
    {
    public:
        BuiltinResources();
        virtual ~BuiltinResources();

        TE_MODULE_STATIC_HEADER_MEMBER(BuiltinResources)

        /** @copydoc Module::OnStartUp */
        void OnStartUp() override;

        /** @copydoc Module::OnShutDown */
        void OnShutDown() override;

        /**	Returns one of the builtin shader types. */
        HShader GetBuiltinShader(BuiltinShader type);

        /** If no material has been specified, it's useful to use the default one */
        HMaterial GetDefaultMaterial() { return _defaultMaterial;  }

        /**	Returns one of the builtin sampler types. */
        SPtr<SamplerState> GetBuiltinSampler(BuiltinSampler type);

        /** Returns default framework icon */
        const PixelData& GetFrameworkIcon();

    public:
        static constexpr const char* MATERIALS_FOLDER = "Data/Materials/";
        static constexpr const char* MESHES_FOLDER = "Data/Meshes/";
        static constexpr const char* SHADERS_FOLDER = "Data/Shaders/";
        static constexpr const char* TEXTURES_FOLDER = "Data/Textures/";
        static constexpr const char* ICONS_FOLDER = "Data/Icons/";

    private:
        void InitGpuPrograms();
        void InitStates();
        void InitShaderDesc();

        void InitSamplers();

        void InitShaderOpaque();
        void InitShaderTransparent(bool cull = true);
        void InitShaderBlit();
        void InitShaderSkybox();
        void InitShaderFXAA();
        void InitShaderToneMapping();
        void InitShaderBloom();
        void InitShaderMotionBlur();
        void InitShaderGaussianBlur();
        void InitShaderPicking();
        void InitShaderSelection();
        void InitShaderHudPicking();
        void InitShaderHudSelection();
        void InitShaderBulletDebug();
        void InitShaderSSAO();
        void InitShaderSSAOBlur();
        void InitShaderSSAODownSample();
        void InitShaderDecal();

        void InitDefaultMaterial();

        void InitFrameworkIcon();

    private:
        HShader _shaderOpaque;
        HShader _shaderTransparent;
        HShader _shaderTransparentCullNone;
        HShader _shaderBlit;
        HShader _shaderSkybox;
        HShader _shaderFXAA;
        HShader _shaderToneMapping;
        HShader _shaderBloom;
        HShader _shaderMotionBlur;
        HShader _shaderGaussianBlur;
        HShader _shaderPicking;
        HShader _shaderHudPicking;
        HShader _shaderSelection;
        HShader _shaderHudSelection;
        HShader _shaderBulletDebug;
        HShader _shaderSSAO;
        HShader _shaderSSAOBlur;
        HShader _shaderSSAODownSample;
        HShader _shaderDecal;

        SPtr<SamplerState> _anisotropicSamplerState = nullptr;
        SPtr<SamplerState> _bilinearSamplerState = nullptr;
        SPtr<SamplerState> _trilinearSamplerState = nullptr;

        SHADER_DESC _forwardShaderDesc;
        SHADER_DESC _blitShaderDesc;
        SHADER_DESC _skyboxShaderDesc;
        SHADER_DESC _FXAAShaderDesc;
        SHADER_DESC _toneMappingShaderDesc;
        SHADER_DESC _bloomShaderDesc;
        SHADER_DESC _motionBlurShaderDesc;
        SHADER_DESC _gaussianBlurShaderDesc;
        SHADER_DESC _pickSelectShaderDesc;
        SHADER_DESC _hudPickSelectShaderDesc;
        SHADER_DESC _bulletDebugShaderDesc;
        SHADER_DESC _ssaoShaderDesc;
        SHADER_DESC _ssaoBlurShaderDesc;
        SHADER_DESC _ssaoDownSampleShaderDesc;
        SHADER_DESC _decalShaderDesc;

        GPU_PROGRAM_DESC _vertexShaderForwardDesc;
        GPU_PROGRAM_DESC _pixelShaderForwardDesc;

        GPU_PROGRAM_DESC _vertexShaderBlitDesc;
        GPU_PROGRAM_DESC _pixelShaderBlitDesc;

        GPU_PROGRAM_DESC _vertexShaderSkyboxDesc;
        GPU_PROGRAM_DESC _pixelShaderSkyboxDesc;

        GPU_PROGRAM_DESC _vertexShaderFXAADesc;
        GPU_PROGRAM_DESC _pixelShaderFXAADesc;

        GPU_PROGRAM_DESC _vertexShaderToneMappingDesc;
        GPU_PROGRAM_DESC _pixelShaderToneMappingDesc;

        GPU_PROGRAM_DESC _vertexShaderBloomDesc;
        GPU_PROGRAM_DESC _pixelShaderBloomDesc;

        GPU_PROGRAM_DESC _vertexShaderMotionBlurDesc;
        GPU_PROGRAM_DESC _pixelShaderMotionBlurDesc;

        GPU_PROGRAM_DESC _vertexShaderGaussianBlurDesc;
        GPU_PROGRAM_DESC _pixelShaderGaussianBlurDesc;

        GPU_PROGRAM_DESC _vertexShaderPickSelectDesc;
        GPU_PROGRAM_DESC _pixelShaderPickSelectDesc;

        GPU_PROGRAM_DESC _vertexShaderHudPickSelectDesc;
        GPU_PROGRAM_DESC _geometryShaderHudPickSelectDesc;
        GPU_PROGRAM_DESC _pixelShaderHudPickSelectDesc;

        GPU_PROGRAM_DESC _vertexShaderBulletDebugDesc;
        GPU_PROGRAM_DESC _geometryShaderBulletDebugDesc;
        GPU_PROGRAM_DESC _pixelShaderBulletDebugDesc;

        GPU_PROGRAM_DESC _vertexShaderPreviewOpaqueDesc;
        GPU_PROGRAM_DESC _pixelShaderPreviewTransparentDesc;

        GPU_PROGRAM_DESC _vertexShaderSSAODesc;
        GPU_PROGRAM_DESC _pixelShaderSSAODesc;

        GPU_PROGRAM_DESC _vertexShaderSSAOBlurDesc;
        GPU_PROGRAM_DESC _pixelShaderSSAOBlurDesc;

        GPU_PROGRAM_DESC _vertexShaderSSAODownSampleDesc;
        GPU_PROGRAM_DESC _pixelShaderSSAODownSampleDesc;

        GPU_PROGRAM_DESC _vertexShaderDecalDesc;
        GPU_PROGRAM_DESC _pixelShaderDecalDesc;

        BLEND_STATE_DESC _blendOpaqueStateDesc;
        BLEND_STATE_DESC _blendTransparentStateDesc;

        RASTERIZER_STATE_DESC _rasterizerStateDesc;
        DEPTH_STENCIL_STATE_DESC _depthStencilStateDesc;

        SAMPLER_STATE_DESC _anisotropicSamplerStateDesc;
        SAMPLER_STATE_DESC _bilinearSamplerStateDesc;
        SAMPLER_STATE_DESC _trilinearSamplerStateDesc;

        HMaterial _defaultMaterial;

        SPtr<PixelData> _frameworkIcon;
    };

    /**	Provides easy access to BuiltinResources. */
    TE_CORE_EXPORT BuiltinResources& gBuiltinResources();
}
