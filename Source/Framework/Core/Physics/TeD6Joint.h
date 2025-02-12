#pragma once

#include "TeCorePrerequisites.h"
#include "Physics/TeJoint.h"

namespace te
{
    /**
     * Represents the most customizable type of joint. This joint type can be used to create all other built-in joint
     * types, and to design your own custom ones, but is less intuitive to use. Allows a specification of a linear
     * constraint (for example for slider), twist constraint (rotating around X) and swing constraint (rotating around Y and
     * Z). It also allows you to constrain limits to only specific axes or completely lock specific axes.
     */
    class TE_CORE_EXPORT D6Joint : public Joint
    {
    public:
        /** @copydoc Joint::Update */
        virtual void Update() = 0;

        /** */
        virtual void SetLowerLinLimit(float lowerLinLimit);

        /** @copydoc SetLowerLinLimit */
        float GetLowerLinLimit() const { return _lowerLinLimit; }

        /** */
        virtual void SetUpperLinLimit(float upperLinLimit);

        /** @copydoc SetUpperLinLimit */
        float GetUpperLinLimit() const { return _upperLinLimit; }

        /** */
        virtual void SetLowerAngLimit(Degree lowerAngLimit);

        /** @copydoc SetLowerAngLimit */
        const Degree GetLowerAngLimit() const { return _lowerAngLimit; }

        /** */
        virtual void SetUpperAngLimit(Degree upperAngLimit);

        /** @copydoc SetUpperAngLimit */
        const Degree GetUpperAngLimit() const { return _upperAngLimit; }

        /** */
        virtual void SetLinearSpring(bool linearSpring);

        /** @copydoc SetLinearSpring */
        bool GetLinearSpring() const { return _linearSpring; }

        /** */
        virtual void SetAngularSpring(bool angularSpring);

        /** @copydoc SetLinearSpring */
        bool GetAngularSpring() const { return _angularSpring; }

        /** */
        virtual void SetLinearStiffness(float linearStiffness);

        /** @copydoc SetLinearStiffness */
        float GetLinearStiffness() const { return _linearStiffness; }

        /** */
        virtual void SetAngularStiffness(float angularStiffness);

        /** @copydoc SetAngularStiffness */
        float GetAngularStiffness() const { return _angularStiffness; }

        /** */
        virtual void SetLinearDamping(float linearDamping);

        /** @copydoc SetLinearDamping */
        float GetLinearDamping() const { return _linearDamping; }

        /** */
        virtual void SetAngularDamping(float angularDamping);

        /** @copydoc SetAngularDamping */
        float GetAngularDamping() const { return _angularDamping; }

        /**
         * Creates a new d6 joint.
         *
         * @param[in]	scene		Scene to which to add the joint.
         */
        static SPtr<D6Joint> Create(PhysicsScene& scene);

    protected:
        D6Joint();

    protected:
        float _lowerLinLimit = 0.0f;
        float _upperLinLimit = 0.0f;
        Degree _lowerAngLimit;
        Degree _upperAngLimit;

        bool _linearSpring = false;
        bool _angularSpring = false;
        float _linearStiffness = 0.0f;
        float _angularStiffness = 0.0f;
        float _linearDamping = 1.0f;
        float _angularDamping = 1.0f;
    };
}
