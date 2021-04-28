#pragma once

#include "TeRenderManPrerequisites.h"
#include "Renderer/TeRenderer.h"
#include "Renderer/TeSkybox.h"
#include "Renderer/TeCamera.h"
#include "Renderer/TeLight.h"
#include "Renderer/TeRenderable.h"
#include "Renderer/TeParamBlocks.h"
#include "Renderer/TeRenderable.h"
#include "TeRendererView.h"
#include "TeRendererLight.h"
#include "TeRendererRenderable.h"

namespace te
{
    struct FrameInfo;

    /** Contains most scene objects relevant to the renderer. */
    struct SceneInfo
    {
        // Cameras and render targets
        Vector<RendererRenderTarget> RenderTargets;
        Vector<RendererView*> Views;
        UnorderedMap<const Camera*, UINT32> CameraToView;

        // Renderables
        Vector<RendererRenderable*> Renderables;
        Vector<RendererRenderable*> RenderablesInstanced;
        Vector<CullInfo> RenderableCullInfos;

        // Lights
        Vector<RendererLight> DirectionalLights;
        Vector<RendererLight> RadialLights;
        Vector<RendererLight> SpotLights;
        Vector<Sphere> RadialLightWorldBounds;
        Vector<Sphere> SpotLightWorldBounds;

        // Buffers for various transient data that gets rebuilt every frame
        //// Rebuilt every frame
        //mutable Vector<bool> RenderableReady; TODO

        // Sky
        Skybox* SkyboxElem = nullptr;

        // FrameBuffer data
        SPtr<GpuParamBlockBuffer> PerFrameParamBuffer;

        // Buffers for various transient data that gets rebuilt every frame
        //// Rebuilt every frame
        mutable Vector<bool> RenderableReady;
    };

    /** Contains information about the scene (e.g. renderables, lights, cameras) required by the renderer. */
    class RendererScene
    {
    public:
        RendererScene(const SPtr<RenderManOptions>& options);
        ~RendererScene();

        /** Returns a modifiable version of SceneInfo. Only to be used by friends who know what they are doing. */
        SceneInfo& GetSceneInfo() { return _info; }

        /** Registers a new camera in the scene. */
        void RegisterCamera(Camera* camera);

        /** Updates information about a previously registered camera. */
        void UpdateCamera(Camera* camera, UINT32 updateFlag);

        /** Removes a camera from the scene. */
        void UnregisterCamera(Camera* camera);

        /** Registers a new light in the scene. */
        void RegisterLight(Light* light);

        /** Updates information about a previously registered light. */
        void UpdateLight(Light* light);

        /** Removes a light from the scene. */
        void UnregisterLight(Light* light);

        /** Registers a new sky texture in the scene. */
        void RegisterSkybox(Skybox* skybox);

        /** Removes a skybox from the scene. */
        void UnregisterSkybox(Skybox* skybox);

        /** Registers a new renderable object in the scene. */
        void RegisterRenderable(Renderable* renderable);

        /** Updates information about a previously registered renderable object. */
        void UpdateRenderable(Renderable* renderable);

        /** Removes a renderable object from the scene. */
        void UnregisterRenderable(Renderable* renderable);

        /** All renderables market as "mergeable" will be merged into several bigger mesh according to their material */
        void BatchRenderables();

        /** Sometimes, we just want to update data on a mesh without removing and adding renderable (heavy operation) */
        void UpdateMeshData(RendererRenderable* rendererRenderable, Renderable* renderable);

        /** Sometimes, mesh is missing on creation, need to be added after */
        void SetMeshData(RendererRenderable* rendererRenderable, Renderable* renderable);

        /** Updates scene according to the newly provided renderer options. */
        void SetOptions(const SPtr<RenderManOptions>& options);

        /** Updates global per frame parameter buffers with new values. To be called at the start of every frame. */
        void SetParamFrameParams(const float& time, const float& delta);

        /** Update data relative to current rendering camera */
        void SetParamCameraParams(const Color& sceneLightColor);

        /** Update data relative to skybox if exists */
        void SetParamSkyboxParams(bool enabled);

        /**
         * Performs necessary per-frame updates to a renderable. This must be called once every frame for every renderable.
         *
         * @param[in]	idx			Index of the renderable to prepare.
         * @param[in]	frameInfo	Global information describing the current frame.
         */
        void PrepareRenderable(UINT32 idx, const FrameInfo& frameInfo);

        /**
         * Performs necessary steps to make a renderable ready for rendering. This must be called at least once every frame
         * for every renderable that will be drawn. Multiple calls for the same renderable during a single frame will result
         * in a no-op.
         *
         * @param[in]	idx			Index of the renderable to prepare.
         * @param[in]	frameInfo	Global information describing the current frame.
         */
        void PrepareVisibleRenderable(UINT32 idx, const FrameInfo& frameInfo);

    private:
        /** Creates a renderer view descriptor for the particular camera. */
        RENDERER_VIEW_DESC CreateViewDesc(Camera* camera) const;

        /**
         * Find the render target the camera belongs to and adds it to the relevant list. If the camera was previously
         * registered with some other render target it will be removed from it and added to the new target.
         */
        void UpdateCameraRenderTargets(Camera* camera, bool remove = false);

    private:
        SceneInfo _info;
        SPtr<RenderManOptions> _options;
    };
}
