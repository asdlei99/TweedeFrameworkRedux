#include "Physics/TePhysics.h"

namespace te
{
    TE_MODULE_STATIC_MEMBER(Physics)

    Physics::Physics(const PHYSICS_INIT_DESC& init)
        : _initDesc(init)
    { }

    Physics& gPhysics()
    {
        return Physics::Instance();
    }
}
