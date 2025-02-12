#pragma once

#include "TeBulletPhysicsPrerequisites.h"
#include "Physics/TeFJoint.h"

namespace te
{
    /** Bullet implementation of an FJoint. */
    class BulletFJoint : public FJoint
    {
    public:
        BulletFJoint(BulletPhysics* physics, BulletScene* scene, BulletJoint* parent);
        virtual ~BulletFJoint();

        BulletJoint* GetJoint() const { return _parent; }

    protected:
        friend class BulletJoint;

        BulletFJoint();

    protected:
        BulletPhysics* _physics;
        BulletScene* _scene;
        BulletJoint* _parent;
    };
}
