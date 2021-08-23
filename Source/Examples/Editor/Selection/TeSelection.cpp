#include "TeSelection.h"

#include "TeSelectionMat.h"
#include "TeHudSelectionMat.h"
#include "Picking/TePickingUtils.h"
#include "Components/TeCCamera.h"
#include "Components/TeCRenderable.h"
#include "Renderer/TeRendererUtility.h"
#include "Renderer/TeRenderer.h"
#include "Scene/TeSceneObject.h"
#include "Mesh/TeMesh.h"

namespace te
{
    Selection::Selection()
        : _selections(gEditor().GetSelectionData())
        , _material(nullptr)
        , _hudMaterial(nullptr)
    { }

    void Selection::Initialize()
    {
        _material = SelectionMat::Get();
        _hudMaterial = HudSelectionMat::Get();

        PickingUtils::CreateHudInstanceBuffer(_instanceBuffer);
    }

    void Selection::Render(const HCamera& camera, const RendererUtility::RenderTextureData& viewportData)
    {
        RenderAPI& rapi = RenderAPI::Instance();
        UINT32 clearBuffers = FBT_DEPTH;

        rapi.SetRenderTarget(viewportData.RenderTex);
        rapi.ClearViewport(clearBuffers, Color::Black);
        Draw(camera, viewportData);
        rapi.SetRenderTarget(nullptr);
    }

    void Selection::Draw(const HCamera& camera, const RendererUtility::RenderTextureData& viewportData)
    {
        RenderAPI& rapi = RenderAPI::Instance();
        Vector<PickingUtils::PerHudInstanceData> instancedElements;

        _material->BindCamera(camera);

        if (!_selections.ClickedComponent && _selections.ClickedSceneObject) // A SceneObject has been selected, we need to highlight all sub elements
        {
            DrawInternal(camera, _selections.ClickedSceneObject, instancedElements);
        }
        else if(_selections.ClickedComponent) // A single Component has been selected, easier
        {
            TypeID_Core type = (TypeID_Core)_selections.ClickedComponent->GetCoreType();

            switch (type)
            {
                case TID_CRenderable:
                {
                    SPtr<CRenderable> renderable = std::static_pointer_cast<CRenderable>(_selections.ClickedComponent);
                    if (renderable->GetActive() && gRendererUtility().DoFrustumCulling(camera, renderable))
                        DrawRenderable(renderable);
                }
                break;

                default:
                break;
            }

            PickingUtils::FillPerInstanceHud(instancedElements, camera, 
                _selections.ClickedComponent->GetHandle(), PickingUtils::RenderType::Selection);
        }

        if (instancedElements.size() > 0)
        {
            _hudMaterial->BindCamera(camera, PickingUtils::RenderType::Selection);

            UINT32 clearBuffers = FBT_DEPTH;
            rapi.ClearViewport(clearBuffers, Color::Black);

            rapi.SetVertexDeclaration(_instanceBuffer.PointVDecl);
            rapi.SetVertexBuffers(0, &_instanceBuffer.PointVB, 1);
            rapi.SetDrawOperation(DOT_POINT_LIST);

            UINT64 elementToDraw = static_cast<UINT64>(instancedElements.size());

            auto iterBegin = instancedElements.begin();
            auto iterRangeStart = iterBegin;
            auto iterRangeEnd = iterBegin + ((elementToDraw >= MAX_HUD_INSTANCED_BLOCK) ? MAX_HUD_INSTANCED_BLOCK : elementToDraw);

            do
            {
                UINT64 elementsDrawn = static_cast<UINT32>(iterRangeEnd - iterRangeStart);

                _hudMaterial->BindHud(iterRangeStart, iterRangeEnd);
                _hudMaterial->Bind();
                rapi.Draw(0, 1, static_cast<UINT32>(elementsDrawn));

                elementToDraw = elementToDraw - elementsDrawn;

                iterRangeStart = iterRangeEnd;
                iterRangeEnd = iterRangeStart + ((elementToDraw >= MAX_HUD_INSTANCED_BLOCK) ? MAX_HUD_INSTANCED_BLOCK : elementToDraw);
            } 
            while (elementToDraw > 0);
        }
    }

    void Selection::DrawInternal(const HCamera& camera, const SPtr<SceneObject>& sceneObject, Vector<PickingUtils::PerHudInstanceData>& instancedElements)
    {
        for (const auto& component : sceneObject->GetComponents())
        {
            TypeID_Core type = (TypeID_Core)component->GetCoreType();

            switch (type)
            {
                case TID_CRenderable:
                {
                    HRenderable renderable = static_object_cast<CRenderable>(component);
                    if (renderable->GetActive() && gRendererUtility().DoFrustumCulling(camera, renderable))
                        DrawRenderable(renderable.GetInternalPtr());
                }
                break;

                default:
                break;
            }

            PickingUtils::FillPerInstanceHud(instancedElements, camera, component, PickingUtils::RenderType::Selection);
        }

        for (const auto& childSO : sceneObject->GetChildren())
            DrawInternal(camera, childSO.GetInternalPtr(), instancedElements);
    }

    void Selection::DrawRenderable(const SPtr<CRenderable>& renderable)
    {
        SPtr<Mesh> mesh = renderable->GetMesh();

        if (mesh)
        {
            _material->BindRenderable(static_object_cast<CRenderable>(renderable->GetHandle()));
            _material->Bind();

            MeshProperties properties = mesh->GetProperties();
            UINT32 numMeshes = properties.GetNumSubMeshes();

            for (UINT32 i = 0; i < numMeshes; i++)
                gRendererUtility().Draw(mesh, properties.GetSubMesh(i), 1);
        }
    }
}
