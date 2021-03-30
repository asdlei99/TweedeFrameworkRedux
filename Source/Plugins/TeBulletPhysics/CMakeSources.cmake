set (TE_BULLETPHYSICS_INC_NOFILTER
    "TeBulletPhysics.h"
    "TeBulletPhysicsPrerequisites.h"
    "TeBulletRigidBody.h"
    "TeBulletSoftBody.h"
    "TeBulletHingeJoint.h"
    "TeBulletSliderJoint.h"
    "TeBulletSphericalJoint.h"
    "TeBulletD6Joint.h"
    "TeBulletConeTwistJoint.h"
)

set (TE_BULLETPHYSICS_SRC_NOFILTER
    "TeBulletPhysicsPlugin.cpp"
    "TeBulletPhysics.cpp"
    "TeBulletRigidBody.cpp"
    "TeBulletSoftBody.cpp"
    "TeBulletHingeJoint.cpp"
    "TeBulletSliderJoint.cpp"
    "TeBulletSphericalJoint.cpp"
    "TeBulletD6Joint.cpp"
    "TeBulletConeTwistJoint.cpp"
)

source_group ("" FILES ${TE_BULLETPHYSICS_SRC_NOFILTER} ${TE_BULLETPHYSICS_INC_NOFILTER})

set (TE_BULLETPHYSICS_SRC
    ${TE_BULLETPHYSICS_INC_NOFILTER}
    ${TE_BULLETPHYSICS_SRC_NOFILTER}
)
