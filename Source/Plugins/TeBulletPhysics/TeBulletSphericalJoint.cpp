#include "TeBulletSphericalJoint.h"
#include "TeBulletPhysics.h"
#include "TeBulletFJoint.h"
#include "TeBulletFBody.h"
#include "Physics/TeRigidBody.h"

namespace te
{
    BulletSphericalJoint::BulletSphericalJoint(BulletPhysics* physics, BulletScene* scene)
        : SphericalJoint()
        , BulletJoint(physics, scene, this)
    {
        _internal = te_new<BulletFJoint>(physics, scene, this);
    }

    BulletSphericalJoint::~BulletSphericalJoint()
    {
        ReleaseJoint();
        te_delete((BulletFJoint*)_internal);
    }

    void BulletSphericalJoint::SetBody(JointBody body, RigidBody* value)
    {
        Joint::SetBody(body, value);
        BuildJoint();
    }

    void BulletSphericalJoint::SetTransform(JointBody body, const Vector3& position, const Quaternion& rotation)
    {
        Joint::SetTransform(body, position, rotation);
        UpdateJoint();
    }

    void BulletSphericalJoint::SetBreakForce(float force)
    {
        Joint::SetBreakForce(force);
    }

    void BulletSphericalJoint::SetBreakTorque(float torque)
    {
        Joint::SetBreakTorque(torque);
    }

    void BulletSphericalJoint::SetEnableCollision(bool collision)
    {
        Joint::SetEnableCollision(collision);
        BuildJoint();
    }

    void BulletSphericalJoint::BuildJoint()
    {
        ReleaseJoint();

        BulletFBody* fBodyAnchor = nullptr;
        BulletFBody* fBodyTarget = nullptr;

        btRigidBody* btBodyAnchor = nullptr;
        btRigidBody* btBodyTarget = nullptr;

        RigidBody* bodyAnchor = _bodies[(int)JointBody::Anchor].BodyElt;
        RigidBody* bodyTarget = _bodies[(int)JointBody::Target].BodyElt;

        if (bodyAnchor)
        {
            fBodyAnchor = static_cast<BulletFBody*>(bodyAnchor->GetInternal());
            if (fBodyAnchor)
                btBodyAnchor = static_cast<btRigidBody*>(fBodyAnchor->GetBody());
        }

        if (bodyTarget)
        {
            fBodyTarget = static_cast<BulletFBody*>(bodyTarget->GetInternal());
            if (fBodyTarget)
                btBodyTarget = static_cast<btRigidBody*>(fBodyTarget->GetBody());
        }

        if (btBodyAnchor)
        {
            Vector3 anchorScaledPosition = _bodies[(int)JointBody::Anchor].Position - bodyAnchor->GetCenterOfMass();
            Vector3 targetScalePosition = btBodyTarget ? _bodies[(int)JointBody::Target].Position - bodyTarget->GetCenterOfMass() : _bodies[(int)JointBody::Target].Position;

            if (!btBodyTarget)
                btBodyTarget = &btTypedConstraint::getFixedBody();

            _btJoint = te_new<btPoint2PointConstraint>(*btBodyAnchor, *btBodyTarget, ToBtVector3(anchorScaledPosition), ToBtVector3(targetScalePosition));

            if (_btJoint)
            {
                _btJoint->setUserConstraintPtr(this);
                _btJoint->enableFeedback(true);
                _btJoint->setEnabled(true);

                _scene->AddJoint(_btJoint, _enableCollision);
            }
        }

        TE_PRINT("BUILD JOINT");
    }

    void BulletSphericalJoint::UpdateJoint()
    {
        TE_PRINT("UPDATE JOINT");

        if (!_btJoint)
        {
            BuildJoint();
            return;
        }

        BulletFBody* fBodyTarget = nullptr;
        btRigidBody* btBodyTarget = nullptr;

        RigidBody* bodyAnchor = _bodies[(int)JointBody::Anchor].BodyElt;
        RigidBody* bodyTarget = _bodies[(int)JointBody::Target].BodyElt;

        if (bodyTarget)
        {
            fBodyTarget = static_cast<BulletFBody*>(bodyTarget->GetInternal());
            if (fBodyTarget)
                btBodyTarget = static_cast<btRigidBody*>(fBodyTarget->GetBody());
        }

        if (bodyAnchor)
        {
            Vector3 anchorScaledPosition = _bodies[(int)JointBody::Anchor].Position - bodyAnchor->GetCenterOfMass();
            Vector3 targetScalePosition = btBodyTarget ? _bodies[(int)JointBody::Target].Position - bodyTarget->GetCenterOfMass() : _bodies[(int)JointBody::Target].Position;

            ((btPoint2PointConstraint*)_btJoint)->setPivotA(ToBtVector3(anchorScaledPosition));
            ((btPoint2PointConstraint*)_btJoint)->setPivotB(ToBtVector3(targetScalePosition));
        }
    }

    void BulletSphericalJoint::ReleaseJoint()
    {
        TE_PRINT("RELEASE JOINT");

        if (!_btJoint)
            return;

        _scene->RemoveJoint(_btJoint);

        te_delete((btPoint2PointConstraint*)_btJoint);
        _btJoint = nullptr;
    }
}
