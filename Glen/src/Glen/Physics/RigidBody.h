# pragma once 

#ifndef RIGIDBODY_H

#include "Glen/Scene/Transform.h"
#include "Collider.h"
#include "PhysicsContext.h"

using namespace physx;

class RigidBody {
private:
	PxRigidDynamic* dynamic;
	// support multiple shapes per rigidBody ?
	PxShape* shape;
public:
	RigidBody(Transform& transform, Collider* collider);
	PxRigidDynamic* getNativeRigidBody();
};

#endif // !RIGIDBODY_H
