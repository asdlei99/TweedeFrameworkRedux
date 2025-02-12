#include "Components/TeCHeightFieldCollider.h"
#include "Components/TeCRigidBody.h"
#include "Scene/TeSceneObject.h"
#include "Scene/TeSceneManager.h"

namespace te
{
    CHeightFieldCollider::CHeightFieldCollider()
        : CCollider((UINT32)TID_CHeightFieldCollider)
    {
        SetName("HeightFieldCollider");
    }

    CHeightFieldCollider::CHeightFieldCollider(const HSceneObject& parent)
        : CCollider(parent, (UINT32)TID_CHeightFieldCollider)
    {
        SetName("HeightFieldCollider");
    }

    SPtr<Collider> CHeightFieldCollider::CreateInternal()
    {
        const SPtr<SceneInstance>& scene = SO()->GetScene();

        SPtr<HeightFieldCollider> collider = HeightFieldCollider::Create(*scene->GetPhysicsScene());
        collider->SetOwner(PhysicsOwnerType::Component, this);

        return collider;
    }

    void CHeightFieldCollider::RestoreInternal()
    {
        CCollider::RestoreInternal();

        SetMinHeight(_minHeight);
        SetMaxHeight(_maxHeight);
    }

    bool CHeightFieldCollider::Clone(const HComponent& c, const String& suffix)
    {
        if (c.Empty())
        {
            TE_DEBUG("Tries to clone a component using an invalid component handle");
            return false;
        }

        return Clone(static_object_cast<CHeightFieldCollider>(c), suffix);
    }

    bool CHeightFieldCollider::Clone(const HHeightFieldCollider& c, const String& suffix)
    {
        if (c.Empty())
        {
            TE_DEBUG("Tries to clone a component using an invalid component handle");
            return false;
        }

        if (CCollider::Clone(static_object_cast<CCollider>(c), suffix))
        {
            _heightField = c->_heightField;
            _minHeight = c->_minHeight;
            _maxHeight = c->_maxHeight;

            return true;
        }

        return false;
    }

    void CHeightFieldCollider::SetHeightField(const HPhysicsHeightField& heightField)
    {
        if (_heightField == heightField)
            return;

        _heightField = heightField;

        if (_internal != nullptr)
        {
            if (_parent != nullptr)
                _parent->RemoveCollider(static_object_cast<CCollider>(GetHandle()));

            GetInternal()->SetHeightField(heightField);

            if (_parent.Empty() || !_heightField.IsLoaded())
                UpdateParentBody();
            else
                _parent->AddCollider(static_object_cast<CCollider>(GetHandle()));
        }
    }

    void CHeightFieldCollider::SetMinHeight(const float& minHeight)
    {
        if (_minHeight == minHeight || minHeight > _maxHeight)
            return;

        _minHeight = minHeight;

        if (_internal != nullptr)
        {
            if (_parent != nullptr)
                _parent->RemoveCollider(static_object_cast<CCollider>(GetHandle()));

            GetInternal()->SetMinHeight(minHeight);

            if (_parent)
                _parent->AddCollider(static_object_cast<CCollider>(GetHandle()));
        }
    }

    void CHeightFieldCollider::SetMaxHeight(const float& maxHeight)
    {
        if (_maxHeight == maxHeight || maxHeight < _minHeight)
            return;

        _maxHeight = maxHeight;

        if (_internal != nullptr)
        {
            if (_parent != nullptr)
                _parent->RemoveCollider(static_object_cast<CCollider>(GetHandle()));

            GetInternal()->SetMaxHeight(maxHeight);

            if (_parent)
                _parent->AddCollider(static_object_cast<CCollider>(GetHandle()));
        }
    }
}
