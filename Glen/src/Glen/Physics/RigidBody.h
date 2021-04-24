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
private:
	PxRigidActor* rigidBody;
	// support multiple shapes per rigidBody ?
	PxShape* shape;
	RigidBodyType type;
public:
	RigidBody(Transform& transform, Collider* collider, RigidBodyType type);
	PxRigidActor* getNativeRigidBody();
	void addForce(glm::vec3 force);
};
#endif // !RIGIDBODY_H
