set (TE_D3D11RENDERAPI_INC_NOFILTER
    "TeD3D11RenderAPIPrerequisites.h"
    "TeD3D11RenderAPIFactory.h"
    "TeD3D11RenderAPI.h"
    "TeD3D11RenderWindow.h"
    "TeD3D11Device.h"
    "TeD3D11DriverList.h"
    "TeD3D11Driver.h"
    "TeD3D11InputLayoutManager.h"
    "TeD3D11HLSLProgramFactory.h"
    "TeD3D11VideoModeInfo.h"
    "TeD3D11Mappings.h"
    "TeD3D11DepthStencilState.h"
    "TeD3D11RasterizerState.h"
    "TeD3D11BlendState.h"
    "TeD3D11SamplerState.h"
    "TeD3D11RenderStateManager.h"
    "TeD3D11TextureManager.h"
    "TeD3D11Texture.h"
    "TeD3D11TextureView.h"
    "TeD3D11RenderTexture.h"
    "TeD3D11GpuProgram.h"
)

set (TE_D3D11RENDERAPI_SRC_NOFILTER
    "TeD3D11RenderAPIFactory.cpp"
    "TeD3D11RenderAPI.cpp"
    "TeD3D11RenderAPIPlugin.cpp"
    "TeD3D11RenderWindow.cpp"
    "TeD3D11Device.cpp"
    "TeD3D11DriverList.cpp"
    "TeD3D11Driver.cpp"
    "TeD3D11InputLayoutManager.cpp"
    "TeD3D11HLSLProgramFactory.cpp"
    "TeD3D11VideoModeInfo.cpp"
    "TeD3D11Mappings.cpp"
    "TeD3D11DepthStencilState.cpp"
    "TeD3D11RasterizerState.cpp"
    "TeD3D11BlendState.cpp"
    "TeD3D11SamplerState.cpp"
    "TeD3D11RenderStateManager.cpp"
    "TeD3D11TextureManager.cpp"
    "TeD3D11Texture.cpp"
    "TeD3D11TextureView.cpp"
    "TeD3D11RenderTexture.cpp"
    "TeD3D11GpuProgram.cpp"
)

source_group ("" FILES ${TE_D3D11RENDERAPI_SRC_NOFILTER} ${TE_D3D11RENDERAPI_INC_NOFILTER})

set (TE_D3D11RENDERAPI_SRC
    ${TE_D3D11RENDERAPI_INC_NOFILTER}
    ${TE_D3D11RENDERAPI_SRC_NOFILTER}
)