#include "Components/TeCCollider.h"
#include "Scene/TeSceneObject.h"
#include "Scene/TeSceneManager.h"
#include "Physics/TePhysics.h"
#include "Components/TeCRigidBody.h"

#include <functional>

using namespace std::placeholders;

namespace te
{
    CCollider::CCollider(UINT32 type)
        : Component(HSceneObject(), type)
        , _internal(nullptr)
    {
        SetName("Collider");
        SetNotifyFlags(TCF_Parent | TCF_Transform);
    }

    CCollider::CCollider(const HSceneObject& parent, UINT32 type)
        : Component(parent, type)
        , _internal(nullptr)
    {
        SetName("Collider");
        SetNotifyFlags(TCF_Parent | TCF_Transform);
    }

    CCollider::~CCollider()
    {
        if (_internal && !_internal->IsDestroyed())
            _internal->Destroy();
    }

    void CCollider::Initialize()
    { 
        RestoreInternal();
        Component::Initialize();
    }

    bool CCollider::Clone(const HCollider& c, const String& suffix)
    {
        if (c.Empty())
        {
            TE_DEBUG("Tries to clone a component using an invalid component handle");
            return false;
        }

        if (Component::Clone(c.GetInternalPtr(), suffix))
        {
            _scale = c->_scale;
            _center = c->_center;
            _position = c->_position;
            _rotation = c->_rotation;

            return true;
        }

        return false;
    }

    void CCollider::SetScale(const Vector3& scale)
    {
        _scale = scale;

        if (_internal != nullptr)
        {
            if (_parent != nullptr)
                _parent->RemoveCollider(static_object_cast<CCollider>(GetHandle()));

            _internal->SetScale(scale);

            if (_parent != nullptr)
                _parent->AddCollider(static_object_cast<CCollider>(GetHandle()));
        }
    }

    void CCollider::SetCenter(const Vector3& center)
    {
        _center = center;

        if (_internal != nullptr)
        {
            if (_parent != nullptr)
                _parent->RemoveCollider(static_object_cast<CCollider>(GetHandle()));

            _internal->SetCenter(center);

            if (_parent != nullptr)
                _parent->AddCollider(static_object_cast<CCollider>(GetHandle()));
        }
    }

    void CCollider::SetPosition(const Vector3& position)
    {
        _position = position;

        if (_internal != nullptr)
        {
            _internal->SetPosition(position);

            if (_parent != nullptr)
            {
                _parent->RemoveCollider(static_object_cast<CCollider>(GetHandle()));
                _parent->AddCollider(static_object_cast<CCollider>(GetHandle()));
            }
        }
    }

    void CCollider::SetRotation(const Quaternion& rotation)
    {
        _rotation = rotation;

        if (_internal != nullptr)
        {
            _internal->SetRotation(rotation);

            if (_parent != nullptr)
            {
                _parent->RemoveCollider(static_object_cast<CCollider>(GetHandle()));
                _parent->AddCollider(static_object_cast<CCollider>(GetHandle()));
            }
        }
    }

    void CCollider::OnDestroyed()
    {
        if (_internal != nullptr)
            DestroyInternal();
    }

    void CCollider::OnDisabled()
    {
        if (_internal != nullptr)
            DestroyInternal();
    }

    void CCollider::OnEnabled()
    {
        RestoreInternal();
    }

    void CCollider::OnTransformChanged(TransformChangedFlags flags)
    {
        if (_internal == nullptr)
            return;

        if ((flags & TCF_Parent) != 0)
            UpdateParentBody();

        // Don't update the transform if it's due to Physics update since then we can guarantee it will remain at the same
        // relative transform to its parent
        if (gPhysics().IsUpdateInProgress())
            return;
    }

    void CCollider::RestoreInternal()
    {
        if (_internal == nullptr)
            _internal = CreateInternal();

        SetPosition(_position);
        SetRotation(_rotation);
        SetCenter(_center);
        SetScale(_scale);

        UpdateParentBody();
    }

    void CCollider::DestroyInternal()
    { 
        if (_parent != nullptr)
            _parent->RemoveCollider(static_object_cast<CCollider>(_thisHandle));

        _parent = nullptr;

        // This should release the last reference and destroy the internal collider
        if (_internal)
        {
            _internal->SetOwner(PhysicsOwnerType::None, nullptr);
            _internal = nullptr;
        }
    }

    void CCollider::SetBody(const HRigidBody& body, bool internal)
    { 
        //if (body == _parent)
        //    return;

        if (_internal != nullptr && !internal)
        {
            if (_parent != nullptr)
                _parent->RemoveCollider(static_object_cast<CCollider>(_thisHandle));

            Body* bodyPtr = nullptr;
            if (body != nullptr)
            {
                bodyPtr = body->GetInternal();
                _internal->SetBody(bodyPtr);

                if (bodyPtr != nullptr)
                    body->AddCollider(static_object_cast<CCollider>(_thisHandle));
            }
        }

        _parent = body;
    }

    void CCollider::UpdateParentBody()
    { 
        HSceneObject currentSO = SO();
        while (currentSO != nullptr)
        {
            HRigidBody parent = static_object_cast<CRigidBody>(currentSO->GetComponent<CRigidBody>());
            if (parent != nullptr)
            {
                if (currentSO->GetActive() && IsValidParent(parent))
                    SetBody(parent);
                else
                    SetBody(HRigidBody());

                return;
            }

            currentSO = currentSO->GetParent();
        }

        // Not found
        SetBody(HRigidBody());
    }
}
