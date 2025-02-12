#pragma once

#include "TeCorePrerequisites.h"
#include "Physics/TePhysicsCommon.h"
#include "Utility/TeModule.h"
#include "Math/TeVector3.h"

#include <cfloat>

namespace te
{
    /** Contains parameters used for initializing the physics system. */
    struct PHYSICS_INIT_DESC
    {
        Vector3 Gravity = Vector3(0.0f, -9.81f, 0.0f); /**< Initial gravity. */
        float AirDensity = 1.2f;
        float WaterDensity = 1.5f;
        float WaterOffset = 0.0f;
        Vector3 WaterNormal = Vector3(0.0f, 1.0f, 0.0f);
        bool SoftBody = true;
    };

    /** Provides global physics settings, factory methods for physics objects and scene queries. */
    class TE_CORE_EXPORT Physics : public Module<Physics>
    {
    public:
        Physics(const PHYSICS_INIT_DESC& init);
        virtual ~Physics() = default;

        TE_MODULE_STATIC_HEADER_MEMBER(Physics)

        /** @copydoc PhysicsMesh::Create */
        virtual SPtr<PhysicsMesh> CreateMesh(const SPtr<MeshData>& meshData) = 0;

        /** @copydoc PhysicsHeightField::Create */
        virtual SPtr<PhysicsHeightField> CreateHeightField(const SPtr<Texture>& texture) = 0;

        /** Creates an object representing the physics scene. Must be manually released via destroyPhysicsScene(). */
        virtual SPtr<PhysicsScene> CreatePhysicsScene() = 0;

        /** Performs any physics operations. Should be called once per frame. */
        virtual void Update() { }

        /** Determines if audio reproduction is paused globally. */
        virtual void SetPaused(bool paused) = 0;

        /** @copydoc SetPaused */
        virtual bool IsPaused() const = 0;

        /** Enable or disable debug information globally */
        virtual void SetDebug(bool debug) = 0;

        /** @copydoc SetDebug */
        virtual bool IsDebug() = 0;

        /** Allow user to see debug information about physical simulation */
        virtual void DrawDebug(const SPtr<Camera>& camera, const SPtr<RenderTarget>& renderTarget) = 0;

        /** Determines gravity to apply to the scene */
        virtual void SetGravity(const Vector3& gravity) = 0;

        /** Determines air density to apply to the scene */
        virtual void SetAirDensity(const float& airDensity) = 0;

        /** Determines water density to apply to the scene */
        virtual void SetWaterDensity(const float& waterDensity) = 0;

        /** Determines water normal to apply to the scene */
        virtual void SetWaterNormal(const Vector3& waterDensity) = 0;

        /** Determines water offset to apply to the scene */
        virtual void SetWaterOffset(const float& waterOffset) = 0;

        /** Checks is the physics simulation update currently in progress. */
        bool IsUpdateInProgress() const { return _updateInProgress; }

        /** Returns configuration of the physic engine */
        const PHYSICS_INIT_DESC& GetDesc() const { return _initDesc; }

    protected:
        bool _updateInProgress = false;
        PHYSICS_INIT_DESC _initDesc;
    };

    /**
     * Physical representation of a scene, allowing creation of new physical objects in the scene and queries against
     * those objects. Objects created in different scenes cannot physically interact with eachother.
     */
    class TE_CORE_EXPORT PhysicsScene
    {
    public:

        /** Manage collision detection inside a scene */
        virtual void TriggerCollisions() = 0;

        /** Report collisions inside a scene */
        virtual void ReportCollisions() = 0;

        /** @copydoc RigidBody::Create */
        virtual SPtr<RigidBody> CreateRigidBody(const HSceneObject& linkedSO) = 0;

        /** @copydoc MeshSoftBody::Create */
        virtual SPtr<MeshSoftBody> CreateMeshSoftBody(const HSceneObject& linkedSO) = 0;

        /** @copydoc EllipsoidSoftBody::Create */
        virtual SPtr<EllipsoidSoftBody> CreateEllipsoidSoftBody(const HSceneObject& linkedSO) = 0;

        /** @copydoc RopeSoftBody::Create */
        virtual SPtr<RopeSoftBody> CreateRopeSoftBody(const HSceneObject& linkedSO) = 0;

        /** @copydoc PatchSoftBody::Create */
        virtual SPtr<PatchSoftBody> CreatePatchSoftBody(const HSceneObject& linkedSO) = 0;

        /** Creates a new cone twist joint. */
        virtual SPtr<ConeTwistJoint> CreateConeTwistJoint() = 0;

        /** Creates a new hinge joint. */
        virtual SPtr<HingeJoint> CreateHingeJoint() = 0;

        /** Creates a new spherical joint. */
        virtual SPtr<SphericalJoint> CreateSphericalJoint() = 0;

        /** Creates a new spherical joint.  */
        virtual SPtr<SliderJoint> CreateSliderJoint() = 0;

        /** Creates a new D6 joint. */
        virtual SPtr<D6Joint> CreateD6Joint() = 0;

        /**
         * Creates a new box collider.
         * 
         * @param[in]	extents		Extents (half size) of the box.
         * @param[in]	position	Position of the collider relative to its parent
         * @param[in]	rotation	Position of the collider relative to its parent
         */
        virtual SPtr<BoxCollider> CreateBoxCollider(const Vector3& extents, const Vector3& position,
            const Quaternion& rotation) = 0;

        /**
         * Creates a new plane collider.
         * 
         * @param[in]	normal		Normal to the plane.
         * @param[in]	position	Position of the collider relative to its parent
         * @param[in]	rotation	Position of the collider relative to its parent
         */
        virtual SPtr<PlaneCollider> CreatePlaneCollider(const Vector3& normal, const Vector3& position,
            const Quaternion& rotation) = 0;

        /**
         * Creates a new sphere collider.
         * 
         * @param[in]	radius		Radius of the sphere geometry.
         * @param[in]	position	Position of the collider.
         * @param[in]	rotation	Rotation of the collider.
         */
        virtual SPtr<SphereCollider> CreateSphereCollider(float radius, const Vector3& position,
            const Quaternion& rotation) = 0;

        /**
         * Creates a new cylinder collider.
         * 
         * @param[in]	extents		Extents (half size) of the cylinder.
         * @param[in]	position	Position of the collider relative to its parent
         * @param[in]	rotation	Position of the collider relative to its parent
         */
        virtual SPtr<CylinderCollider> CreateCylinderCollider(const Vector3& extents, const Vector3& position,
            const Quaternion& rotation) = 0;

        /**
         * Creates a new capsule collider.
         * 
         * @param[in]	radius		Radius of the capsule
         * @param[in]	height		Height of the capsule
         * @param[in]	position	Position of the collider relative to its parent
         * @param[in]	rotation	Position of the collider relative to its parent
         */
        virtual SPtr<CapsuleCollider> CreateCapsuleCollider(float radius, float height, const Vector3& position,
            const Quaternion& rotation) = 0;

        /**
         * Creates a new mesh collider.
         * 
         * @param[in]	position	Position of the collider relative to its parent
         * @param[in]	rotation	Position of the collider relative to its parent
         */
        virtual SPtr<MeshCollider> CreateMeshCollider(const Vector3& position, const Quaternion& rotation) = 0;

        /**
         * Creates a new cone collider.
         * 
         * @param[in]	radius		Radius of the cone
         * @param[in]	height		Height of the cone
         * @param[in]	position	Position of the collider relative to its parent
         * @param[in]	rotation	Position of the collider relative to its parent
         */
        virtual SPtr<ConeCollider> CreateConeCollider(float radius, float height, const Vector3& position,
            const Quaternion& rotation) = 0;

        /**
         * Creates a new height map collider.
         *
         * @param[in]	position	Position of the collider relative to its parent
         * @param[in]	rotation	Position of the collider relative to its parent
         */
        virtual SPtr<HeightFieldCollider> CreateHeightFieldCollider(const Vector3& position, const Quaternion& rotation) = 0;

        /**
         * Checks does the ray hit the provided collider.
         *
         * @param[in]	origin		Origin of the ray to check.
         * @param[in]	unitDir		Unit direction of the ray to check.
         * @param[out]	hit			Information about the hit. Valid only if the method returns true.
         * @param[in]	maxDist		Maximum distance from the ray origin to search for hits.
         * @return					True if the ray has hit the collider.
         */
        virtual bool RayCast(const Vector3& origin, const Vector3& unitDir, PhysicsQueryHit& hit,
            float maxDist = FLT_MAX) const = 0;

        /**
         * Checks does the ray hit the provided collider.
         *
         * @param[in]	origin		Origin of the ray to check.
         * @param[in]	unitDir		Unit direction of the ray to check.
         * @param[out]	hits		Information about all the hits. Valid only if the method returns true.
         * @param[in]	maxDist		Maximum distance from the ray origin to search for hits.
         * @return					True if the ray has hit the collider.
         */
        virtual bool RayCast(const Vector3& origin, const Vector3& unitDir, Vector<PhysicsQueryHit>& hits,
            float maxDist = FLT_MAX) const = 0;

    protected:
        PhysicsScene() = default;
        virtual ~PhysicsScene() = default;

    protected:
        PhysicsDebug* _debug = nullptr;

    };

    /** Provides easier access to Physics. */
    TE_CORE_EXPORT Physics& gPhysics();
}
