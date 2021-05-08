# pragma once 

#ifndef RIGIDBODY_H

#include "Glen/Scene/Transform.h"
#include "Collider.h"
#include "PhysicsContext.h"
#include "Glen/Core/Logger.h"
#include "Glen/Core/Core.h"
#include "Glen/Core/EngineContext.h"

using namespace physx;

enum class RigidBodyType {
	Dynamic,
	Static
};

class GLN_API RigidBody {
public:
	PxRigidActor* actorRef;
	RigidBodyType type;
	// Put this into a rigidBody component
	bool isAttached = false;

	RigidBody();
	PxRigidActor* getNativeRigidBody();
	void addForce(glm::vec3 force);
};
#endif // !RIGIDBODY_H
