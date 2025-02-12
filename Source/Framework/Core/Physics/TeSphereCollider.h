#pragma once

#include "TeCorePrerequisites.h"
#include "Physics/TeCollider.h"

namespace te
{
    class PhysicsScene;

    /** Collider with sphere geometry. */
    class TE_CORE_EXPORT SphereCollider : public Collider
    {
    public:
        /** Determines the radius of the sphere geometry. */
        virtual void SetRadius(float radius) = 0;

        /** @copydoc SetRadius */
        virtual float GetRadius() const = 0;

        /**
         * Creates a new sphere collider.
         *
         * @param[in]	scene		Scene into which to add the collider to.
         * @param[in]	radius		Radius of the sphere geometry.
         * @param[in]	position	Position of the collider.
         * @param[in]	rotation	Rotation of the collider.
         */
        static SPtr<SphereCollider> Create(PhysicsScene& scene, float radius = 0.0f,
            const Vector3& position = Vector3::ZERO, const Quaternion& rotation = Quaternion::IDENTITY);

    protected:
        SphereCollider();
    };
}
