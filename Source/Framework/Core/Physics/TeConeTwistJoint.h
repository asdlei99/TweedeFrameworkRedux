#pragma once

#include "TeCorePrerequisites.h"
#include "Physics/TeJoint.h"

namespace te
{
    /**
     * To create ragdolls, the conve twist constraint is very useful for limbs like the upper arm. It is a special 
     * point to point constraint that adds cone and twist axis limits. The x-axis serves as twist axis.
     */
    class TE_CORE_EXPORT ConeTwistJoint : public Joint
    {
    public:
        ConeTwistJoint() { }
        virtual ~ConeTwistJoint() = default;

        /** @copydoc Joint::Update */
        virtual void Update() = 0;

        /**
         * Creates a new cone twist joint.
         *
         * @param[in]	scene		Scene to which to add the joint.
         * @param[in]	desc		Settings describing the joint.
         */
        static SPtr<ConeTwistJoint> Create(PhysicsScene& scene);
    };
}
