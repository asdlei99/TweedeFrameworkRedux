#include "TeBulletSphericalJoint.h"
#include "TeBulletPhysics.h"
#include "TeBulletFJoint.h"
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
        _internal = nullptr;
    }

    void BulletSphericalJoint::Update()
    {
        if (!_isBroken && IsJointBroken())
        {
            _isBroken = true;
            OnJointBreak();
        }
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

    void BulletSphericalJoint::SetOffsetPivot(JointBody body, const Vector3& offset)
    {
        Joint::SetOffsetPivot(body, offset);
        BuildJoint();
    }

    void BulletSphericalJoint::SetIsBroken(bool isBroken)
    {
        Joint::SetIsBroken(isBroken);
        BuildJoint();
    }

    void BulletSphericalJoint::BuildJoint()
    {
        ReleaseJoint();

        if (_isBroken)
            return;

        RigidBody* bodyAnchor = _bodies[(int)JointBody::Anchor].BodyElt;
        RigidBody* bodyTarget = _bodies[(int)JointBody::Target].BodyElt;

        btRigidBody* btBodyAnchor = static_cast<btRigidBody*>(GetBtCollisionObject(&_bodies[(int)JointBody::Anchor]));
        btRigidBody* btBodyTarget = static_cast<btRigidBody*>(GetBtCollisionObject(&_bodies[(int)JointBody::Target]));

        if (btBodyAnchor)
        {
            Vector3 anchorScaledPosition = GetAnchorScaledPosisition(bodyAnchor, _bodies, _offsetPivots);
            Vector3 targetScaledPosition = GetTargetScaledPosisition(btBodyTarget, bodyTarget, _bodies, _offsetPivots);
                
            if (!btBodyTarget)
                btBodyTarget = &btTypedConstraint::getFixedBody();

            _btJoint = te_new<btPoint2PointConstraint>(*btBodyAnchor, *btBodyTarget, ToBtVector3(anchorScaledPosition), ToBtVector3(targetScaledPosition));

            if (_btJoint)
            {
                _btFeedBack = te_new<btJointFeedback>();

                _btJoint->setUserConstraintPtr(this);
                _btJoint->enableFeedback(true);
                _btJoint->setEnabled(true);
                _btJoint->setJointFeedback(_btFeedBack);

                _scene->AddJoint(_btJoint, _enableCollision);
            }
        }
    }

    void BulletSphericalJoint::UpdateJoint()
    {
        if (!_btJoint)
        {
            BuildJoint();
            return;
        }

        RigidBody* bodyAnchor = _bodies[(int)JointBody::Anchor].BodyElt;
        RigidBody* bodyTarget = _bodies[(int)JointBody::Target].BodyElt;

        btRigidBody* btBodyTarget = static_cast<btRigidBody*>(GetBtCollisionObject(&_bodies[(int)JointBody::Target]));

        if (bodyAnchor)
        {
            Vector3 anchorScaledPosition = GetAnchorScaledPosisition(bodyAnchor, _bodies, _offsetPivots);
            Vector3 targetScaledPosition = GetTargetScaledPosisition(btBodyTarget, bodyTarget, _bodies, _offsetPivots);

            ((btPoint2PointConstraint*)_btJoint)->setPivotA(ToBtVector3(anchorScaledPosition));
            ((btPoint2PointConstraint*)_btJoint)->setPivotB(ToBtVector3(targetScaledPosition));
        }
    }

    void BulletSphericalJoint::ReleaseJoint()
    {
        if (!_btJoint)
            return;

        _scene->RemoveJoint(_btJoint);

        te_delete((btPoint2PointConstraint*)_btJoint);
        te_delete(_btFeedBack);

        _btJoint = nullptr;
        _btFeedBack = nullptr;
    }
}
