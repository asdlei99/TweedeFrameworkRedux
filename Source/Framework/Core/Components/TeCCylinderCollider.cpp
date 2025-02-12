#include "Components/TeCCylinderCollider.h"
#include "Components/TeCRigidBody.h"
#include "Scene/TeSceneObject.h"
#include "Scene/TeSceneManager.h"

namespace te
{
    CCylinderCollider::CCylinderCollider()
        : CCollider((UINT32)TID_CCylinderCollider)
    {
        SetName("CylinderCollider");
    }

    CCylinderCollider::CCylinderCollider(const HSceneObject& parent)
        : CCollider(parent, (UINT32)TID_CCylinderCollider)
    {
        SetName("CylinderCollider");
    }

    SPtr<Collider> CCylinderCollider::CreateInternal()
    {
        const SPtr<SceneInstance>& scene = SO()->GetScene();

        SPtr<CylinderCollider> collider = CylinderCollider::Create(*scene->GetPhysicsScene());
        collider->SetOwner(PhysicsOwnerType::Component, this);

        return collider;
    }

    void CCylinderCollider::RestoreInternal()
    {
        CCollider::RestoreInternal();

        SetExtents(_extents);
    }

    bool CCylinderCollider::Clone(const HComponent& c, const String& suffix)
    {
        if (c.Empty())
        {
            TE_DEBUG("Tries to clone a component using an invalid component handle");
            return false;
        }

        return Clone(static_object_cast<CCylinderCollider>(c), suffix);
    }

    bool CCylinderCollider::Clone(const HCylinderCollider& c, const String& suffix)
    {
        if (c.Empty())
        {
            TE_DEBUG("Tries to clone a component using an invalid component handle");
            return false;
        }

        if (CCollider::Clone(static_object_cast<CCollider>(c), suffix))
        {
            _extents = c->_extents;
            return true;
        }

        return false;
    }

    void CCylinderCollider::SetExtents(const Vector3& extents)
    {
        Vector3 clampedExtents = Vector3::Max(extents, Vector3(0.01f, 0.01f, 0.01f));
        _extents = clampedExtents;

        if (_internal != nullptr)
        {
            if (_parent != nullptr)
                _parent->RemoveCollider(static_object_cast<CCollider>(GetHandle()));

            GetInternal()->SetExtents(clampedExtents);

            if (_parent != nullptr)
                _parent->AddCollider(static_object_cast<CCollider>(GetHandle()));
        }
    }
}
