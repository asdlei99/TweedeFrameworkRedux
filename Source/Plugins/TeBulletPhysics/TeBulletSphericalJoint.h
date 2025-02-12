#pragma once

#include "TeBulletPhysicsPrerequisites.h"
#include "Physics/TeSphericalJoint.h"
#include "TeBulletJoint.h"

namespace te
{
    /** Bullet implementation of a Spherical joint. */
    class BulletSphericalJoint : public SphericalJoint, public BulletJoint
    {
    public:
        BulletSphericalJoint(BulletPhysics* physics, BulletScene* scene);
        virtual ~BulletSphericalJoint();

        /** @copydoc Joint::Update */
        void Update() override;

        /** @copydoc Joint::SetBody */
        void SetBody(JointBody body, RigidBody* value) override;

        /** @copydoc Joint::SetTransform  */
        void SetTransform(JointBody body, const Vector3& position, const Quaternion& rotation) override;

        /** @copydoc Joint::SetBreakForce */
        void SetBreakForce(float force) override;

        /** @copydoc Joint::SetBreakTorque */
        void SetBreakTorque(float torque) override;

        /** @copydoc Joint::SetEnableCollision */
        void SetEnableCollision(bool collision) override;

        /** @copydoc Joint::SetOffsetPivot */
        void SetOffsetPivot(JointBody body, const Vector3& offset) override;

        /** @copydoc Joint::IsBroken */
        void SetIsBroken(bool isBroken) override;

    private:
        /** @copydoc BulletJoint::BuildJoint */
        void BuildJoint() override;

        /** @copydoc BulletJoint::UpdateJoint */
        void UpdateJoint() override;

        /** @copydoc BulletJoint::ReleaseJoint */
        void ReleaseJoint() override;
    };
}
