#pragma once

#include "TeCorePrerequisites.h"
#include "TeCoreApplication.h"
#include "Material/TeMaterial.h"

namespace te
{
    /**
     * Represents the primary entry point for the core systems. Handles start-up, shutdown, primary loop and allows you to
     * load and unload plugins.
     */
    class Application : public CoreApplication
    {
    public:
        Application(START_UP_DESC desc) : CoreApplication(desc) {}
        virtual ~Application() = default;

        TE_MODULE_STATIC_HEADER_MEMBER(Application)

        /** Starts the framework. If using a custom Application system, provide it as a template parameter. */
        template<class T = Application>
        static void StartUp(const START_UP_DESC& desc)
        {
            CoreApplication::StartUp<T>(desc);
        }

    protected:
        /** @copydoc CoreApplication::PostStartUp */
        void PostStartUp() override;

        /** @copydoc CoreApplication::PreShutDown */
        void PreShutDown() override;

        /** @copydoc CoreApplication::PreUpdate */
        void PreUpdate() override;

        /** @copydoc CoreApplication::PreUpdate */
        void PostUpdate() override;

    protected:
        struct SponzaMaterialData
        {
            String Name;
            String Diffuse;
            String Emissive;
            String Normal;
            String Specular;
            String Bump;
            String Parallax;
            String Transparency;
            String Occlusion;
            String Reflection;
            float  Opacity = 1.0f;
            float  AlphaTreshold = 0.1f;
            Color EmissiveColor = Color(0.0f, 0.0f, 0.0f, 1.0f);

            HTexture DiffuseTexture;
            HTexture EmissiveTexture;
            HTexture NormalTexture;
            HTexture SpecularTexture;
            HTexture BumpTexture;
            HTexture ParallaxTexture;
            HTexture TransparencyTexture;
            HTexture OcclusionTexture;
            HTexture ReflectionTexture;

            HMaterial MaterialElement;

            MaterialProperties MaterialProp;
        };

        void InitInputHandling();
        void InitShader();
        void InitMaterials();
        void InitMesh();
        void InitScene();

    protected:
#if TE_PLATFORM == TE_PLATFORM_WIN32
        HShader _shaderOpaque;
        HShader _shaderTransparent;

        Vector<SponzaMaterialData> _materials;
        HMaterial _lightMaterial;

        HTexture _loadedSkyboxTexture;

        HCamera _sceneCamera;
        HCameraFlyer _sceneCameraFlyer;
        HMesh _sponzaMesh;
        HMesh _lightMesh;
        HSkybox _skybox;
        HLight _directionalLight;
        HRenderable _sponzaRenderable;

        HSceneObject _sceneCameraSO;
        HSceneObject _sceneSponzaSO;
        HSceneObject _sceneSkyboxSO;
        HSceneObject _sceneDirectionalLightSO;
        Vector<HSceneObject> _scenePointLightSOs;
#endif
    };
}
