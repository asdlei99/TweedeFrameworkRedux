set (TE_CORE_INC_RENDERAPI
    "Core/RenderAPI/TeRenderAPI.h"
    "Core/RenderAPI/TeRenderWindow.h"
    "Core/RenderAPI/TeVideoMode.h"
    "Core/RenderAPI/TeRenderAPIFactory.h"
    "Core/RenderAPI/TeRenderTarget.h"
    "Core/RenderAPI/TeVertexBuffer.h"
    "Core/RenderAPI/TeVertexDeclaration.h"
    "Core/RenderAPI/TeCommonTypes.h"
)
set (TE_CORE_SRC_RENDERAPI
    "Core/RenderAPI/TeRenderAPI.cpp"
    "Core/RenderAPI/TeRenderWindow.cpp"
    "Core/RenderAPI/TeVideoMode.cpp"
    "Core/RenderAPI/TeRenderTarget.cpp"
    "Core/RenderAPI/TeHardwareBuffer.h"
    "Core/RenderAPI/TeVertexBuffer.h"
    "Core/RenderAPI/TeVertexDeclaration.h"
)

set (TE_CORE_INC_RENDERER
    "Core/Renderer/TeRenderer.h"
    "Core/Renderer/TeRendererFactory.h"
    "Core/Renderer/TeCamera.h"
    "Core/Renderer/TeViewport.h"
    "Core/Renderer/TeCoreObject.h"
)
set (TE_CORE_SRC_RENDERER
    "Core/Renderer/TeRenderer.cpp"
    "Core/Renderer/TeCamera.cpp"
    "Core/Renderer/TeViewport.cpp"
    "Core/Renderer/TeCoreObject.cpp"
)

set (TE_CORE_INC_SCENE
    "Core/Scene/TeSceneActor.h"
    "Core/Scene/TeSceneManager.h"
)
set (TE_CORE_SRC_SCENE
    "Core/Scene/TeSceneActor.cpp"
    "Core/Scene/TeSceneManager.cpp"
)

set(TE_CORE_INC_PLATFORM
    "Core/Platform/TePlatform.h"
)
set(TE_CORE_SRC_PLATFORM 
)

set (TE_CORE_INC_PLATFORM_WIN32
    "Core/Private/Win32/TeWin32Window.h"
    "Core/Private/Win32/TeWin32Input.h"
    "Core/Private/Win32/TeWin32Platform.h"
)
set (TE_CORE_SRC_PLATFORM_WIN32
    "Core/Private/Win32/TeWin32Window.cpp"
    "Core/Private/Win32/TeWin32Input.cpp"
    "Core/Private/Win32/TeWin32GamePad.cpp"
    "Core/Private/Win32/TeWin32Keyboard.cpp"
    "Core/Private/Win32/TeWin32Mouse.cpp"
    "Core/Private/Win32/TeWin32Platform.cpp"
)

set (TE_CORE_INC_PLATFORM_LINUX
    "Core/Private/Linux/TeLinuxWindow.h"
    "Core/Private/Linux/TeLinuxInput.h"
    "Core/Private/Linux/TeLinuxPlatform.h"
)
set (TE_CORE_SRC_PLATFORM_LINUX
    "Core/Private/Linux/TeLinuxWindow.cpp"
    "Core/Private/Linux/TeLinuxInput.cpp"
    "Core/Private/Linux/TeLinuxGamePad.cpp"
    "Core/Private/Linux/TeLinuxKeyboard.cpp"
    "Core/Private/Linux/TeLinuxMouse.cpp"
    "Core/Private/Linux/TeLinuxPlatform.cpp"
)

set(TE_CORE_INC_NOFILTER
	"Core/TeCoreApplication.h"
	"Core/TeCorePrerequisites.h"
	"Core/TeEngineConfig.h"
)
set(TE_CORE_SRC_NOFILTER
	"Core/TeCoreApplication.cpp"
)

set (TE_CORE_INC_INPUT
    "Core/Input/TeInput.h"
    "Core/Input/TeGamePad.h"
    "Core/Input/TeKeyboard.h"
    "Core/Input/TeMouse.h"
    "Core/Input/TeInputData.h"
    "Core/Input/TeVirtualInput.h"
    "Core/Input/TeInputConfiguration.h"
)
set (TE_CORE_SRC_INPUT
    "Core/Input/TeInput.cpp"
    "Core/Input/TeVirtualInput.cpp"
    "Core/Input/TeInputConfiguration.cpp"
)

set (TE_CORE_INC_MANAGER
    "Core/Manager/TeRenderAPIManager.h"
    "Core/Manager/TeRendererManager.h"
    "Core/Manager/TePluginManager.h"
)
set (TE_CORE_SRC_MANAGER
    "Core/Manager/TeRenderAPIManager.cpp"
    "Core/Manager/TeRendererManager.cpp"
)

set (TE_CORE_INC_IMPORTER
    "Core/Importer/TeImporter.h"
    "Core/Importer/TeImportOptions.h"
    "Core/Importer/TeBaseImporter.h"
    "Core/Importer/TeTextureImportOptions.h"
    "Core/Importer/TeMeshImportOptions.h"
)
set (TE_CORE_SRC_IMPORTER
    "Core/Importer/TeImporter.cpp"
    "Core/Importer/TeImportOptions.cpp"
    "Core/Importer/TeBaseImporter.cpp"
    "Core/Importer/TeTextureImportOptions.cpp"
    "Core/Importer/TeMeshImportOptions.cpp"
)

set (TE_CORE_INC_IMAGE
    "Core/Image/TeTexture.h"
)
set (TE_CORE_SRC_IMAGE
    "Core/Image/TeTexture.cpp"
)

set (TE_CORE_INC_MESH
    "Core/Mesh/TeMesh.h"
)
set (TE_CORE_SRC_MESH
    "Core/Mesh/TeMesh.cpp"
)

set (TE_CORE_INC_RESOURCE
    "Core/Resources/TeResource.h"
    "Core/Resources/TeResourceManager.h"
    "Core/Resources/TeResourceHandle.h"
)
set (TE_CORE_SRC_RESOURCE
    "Core/Resources/TeResource.cpp"
    "Core/Resources/TeResourceManager.cpp"
    "Core/Resources/TeResourceHandle.cpp"
)

if (WIN32)
    list(APPEND TE_CORE_INC_PLATFORM ${TE_CORE_INC_PLATFORM_WIN32})
    list(APPEND TE_CORE_SRC_PLATFORM ${TE_CORE_SRC_PLATFORM_WIN32})
elseif (LINUX)
    list(APPEND TE_CORE_INC_PLATFORM ${TE_CORE_INC_PLATFORM_LINUX})
    list(APPEND TE_CORE_SRC_PLATFORM ${TE_CORE_SRC_PLATFORM_LINUX})
endif ()

list(APPEND TE_CORE_INC_PLATFORM ${TE_CORE_INC_PLATFORM})
list(APPEND TE_CORE_SRC_PLATFORM ${TE_CORE_SRC_PLATFORM})

source_group("Core\\RenderAPI" FILES ${TE_CORE_INC_RENDERAPI} ${TE_CORE_SRC_RENDERAPI})
source_group("Core\\Renderer" FILES ${TE_CORE_INC_RENDERER} ${TE_CORE_SRC_RENDERER})
source_group("Core\\Platform" FILES ${TE_CORE_INC_PLATFORM} ${TE_CORE_SRC_PLATFORM})
source_group("Core\\Input" FILES ${TE_CORE_INC_INPUT} ${TE_CORE_SRC_INPUT})
source_group("Core\\Manager" FILES ${TE_CORE_INC_MANAGER} ${TE_CORE_SRC_MANAGER})
source_group("Core\\Importer" FILES ${TE_CORE_INC_IMPORTER} ${TE_CORE_SRC_IMPORTER})
source_group("Core\\Image" FILES ${TE_CORE_INC_IMAGE} ${TE_CORE_SRC_IMAGE})
source_group("Core\\Mesh" FILES ${TE_CORE_INC_MESH} ${TE_CORE_SRC_MESH})
source_group("Core\\Resources" FILES ${TE_CORE_INC_RESOURCE} ${TE_CORE_SRC_RESOURCE})
source_group("Core\\Scene" FILES ${TE_CORE_INC_SCENE} ${TE_CORE_SRC_SCENE})
source_group("Core" FILES ${TE_CORE_INC_NOFILTER} ${TE_CORE_SRC_NOFILTER})

set(TE_CORE_SRC
    ${TE_CORE_SRC_RENDERAPI}
    ${TE_CORE_INC_RENDERAPI}
    ${TE_CORE_SRC_RENDERER}
    ${TE_CORE_INC_RENDERER}
    ${TE_CORE_SRC_PLATFORM}
    ${TE_CORE_INC_PLATFORM}
    ${TE_CORE_SRC_NOFILTER}
    ${TE_CORE_INC_NOFILTER}
    ${TE_CORE_INC_INPUT}
    ${TE_CORE_SRC_INPUT}
    ${TE_CORE_INC_MANAGER}
    ${TE_CORE_SRC_MANAGER}
    ${TE_CORE_INC_IMPORTER}
    ${TE_CORE_SRC_IMPORTER}
    ${TE_CORE_INC_IMAGE}
    ${TE_CORE_SRC_IMAGE}
    ${TE_CORE_INC_MESH}
    ${TE_CORE_SRC_MESH}
    ${TE_CORE_INC_RESOURCE}
    ${TE_CORE_SRC_RESOURCE}
    ${TE_CORE_INC_SCENE}
    ${TE_CORE_SRC_SCENE}
)