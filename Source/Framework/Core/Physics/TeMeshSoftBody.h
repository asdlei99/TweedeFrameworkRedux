#pragma once

#include "TeCorePrerequisites.h"
#include "Physics/TeSoftBody.h"

namespace te
{
    /**
     * A SoftBody built using a PhysicsMesh
     */
    class TE_CORE_EXPORT MeshSoftBody : public SoftBody
    {
    public:
        /**
         * Creates a new mesh softbody.
         *
         * @param[in]	linkedSO	Scene object that owns this softbody. All physics updates applied to this object
         *							will be transfered to this scene object (the movement/rotation resulting from
         *							those updates).
         */
        static SPtr<MeshSoftBody> Create(const HSceneObject& linkedSO);

        /**
         * Sets a mesh that represents the softbody geometry.
         */
        virtual void SetMesh(const HPhysicsMesh& mesh) = 0;

        /**
         * Get the mesh that represents the softbody geometry.
         */
        const HPhysicsMesh& GetMesh() const { return _mesh; }

    protected:
        /**
         * Constructs a new MeshSoftBody.
         *
         * @param[in]	linkedSO	Scene object that owns this softbody. All physics updates applied to this object
         *							will be transfered to this scene object (the movement/rotation resulting from
         *							those updates).
         */
        explicit MeshSoftBody(const HSceneObject& linkedSO);

    protected:
        HPhysicsMesh _mesh;
    };
}
