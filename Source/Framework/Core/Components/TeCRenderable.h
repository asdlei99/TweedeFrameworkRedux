#pragma once

#include "TeCorePrerequisites.h"
#include "Renderer/TeRenderable.h"
#include "Scene/TeComponent.h"

namespace te
{
    /**
     * @copydoc	Renderable
     *
     * @note	Wraps Renderable as a Component.
     */
    class TE_CORE_EXPORT CRenderable : public Component
    {
    public:
        virtual ~CRenderable() = default;

        /** @copydoc Component::Initialize */
        void Initialize() override;

        static UINT32 GetComponentType() { return TID_CRenderable; }

        /** @copydoc Renderable::SetMesh */
        void SetMesh(HMesh mesh) { _internal->SetMesh(mesh.GetInternalPtr()); }

        /** @copydoc Renderable::SetMesh */
        void SetMesh(SPtr<Mesh> mesh) { _internal->SetMesh(mesh); }

        /** @copydoc Renderable::GetMesh */
        SPtr<Mesh> GetMesh() const { return _internal->GetMesh(); }

        /** @copydoc Renderable::SetMaterial */
        void SetMaterial(UINT32 idx, HMaterial material) { _internal->SetMaterial(idx, material.GetInternalPtr()); }

        /** @copydoc Renderable::SetMaterial */
        void SetMaterial(HMaterial material) { _internal->SetMaterial(material.GetInternalPtr()); }

        /** @copydoc Renderable::SetMaterial */
        void SetMaterial(UINT32 idx, SPtr<Material> material) { _internal->SetMaterial(idx, material); }

        /** @copydoc Renderable::SetMaterial */
        void SetMaterial(SPtr<Material> material) { _internal->SetMaterial(material); }

        /** @copydoc Renderable::GetMaterial */
        SPtr<Material> GetMaterial(UINT32 idx) const { return _internal->GetMaterial(idx); }

        /** @copydoc Renderable::SetMaterials */
        void SetMaterials(const Vector<SPtr<Material>>& materials) { _internal->SetMaterials(materials); }

        /** @copydoc Renderable::SetMaterials */
        void SetMaterials(const Vector<HMaterial>& materials);

        /** @copydoc Renderable::GetMaterials */
        const Vector<SPtr<Material>>& GetMaterials() { return _internal->GetMaterials(); }

        /** @copydoc Renderable::SetCullDistanceFactor */
        void SetCullDistanceFactor(float factor) { _internal->SetCullDistanceFactor(factor); }

        /** @copydoc Renderable::GetCullDistanceFactor */
        float GetCullDistanceFactor() const { return _internal->GetCullDistanceFactor(); }

        /** @copydoc Renderable::SetLayer */
        void SetLayer(UINT64 layer) { _internal->SetLayer(layer); }

        /** @copydoc Renderable::GetLayer */
        UINT64 GetLayer() const { return _internal->GetLayer(); }

        /**	Gets world bounds of the mesh rendered by this object. */
        Bounds GetBounds() const;

        /** @copydoc Component::CalculateBounds */
        bool CalculateBounds(Bounds& bounds) override;

         /** Returns the internal renderable that is used for majority of operations by this component. */
        SPtr<Renderable> _getInternal() const { return _internal; }

    protected:
        mutable SPtr<Renderable> _internal;

    protected:
        friend class SceneObject;

        CRenderable(const HSceneObject& parent);

        /** @copydoc Component::_instantiate */
        void _instantiate() override;

        /** @copydoc Component::OnInitialized */
        void OnCreated() override;

        /** @copydoc Component::OnInitialized */
        void OnInitialized() override;

        /** @copydoc Component::OnTransformChanged */
        void OnTransformChanged(TransformChangedFlags flags) override;

        /** @copydoc Component::OnDestroyed */
        void OnDestroyed() override;

    public:
        /** @copydoc Component::Update */
        void Update() override { }

    protected:
        CRenderable();
    };
}
