#include "Physics/TeCollider.h"
#include "Physics/TePhysics.h"
#include "Physics/TeFCollider.h"

namespace te
{
    Vector3 Collider::GetPosition() const
    {
        return _internal->GetPosition();
    }

    Quaternion Collider::GetRotation() const
    {
        return _internal->GetRotation();
    }

    void Collider::SetTransform(const Vector3& pos, const Quaternion& rot)
    {
        _internal->SetTransform(pos, rot);
    }

    void Collider::SetScale(const Vector3& scale)
    {
        _scale = scale;
    }

    Vector3 Collider::GetScale() const
    {
        return _scale;
    }

    void Collider::SetIsTrigger(bool value)
    {
        _internal->SetIsTrigger(value);
    }

    bool Collider::GetIsTrigger() const
    {
        return _internal->GetIsTrigger();
    }

    void Collider::SetCollisionReportMode(CollisionReportMode mode)
    {
        _internal->SetCollisionReportMode(mode);
    }

    CollisionReportMode Collider::GetCollisionReportMode() const
    {
        return _internal->GetCollisionReportMode();
    }

    void Collider::SetRigidBody(RigidBody* value)
    {
        _internal->SetIsStatic(value == nullptr);
        _rigidBody = value;
    }
}
