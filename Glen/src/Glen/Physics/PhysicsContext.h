#pragma once 

#ifndef PHYSICS_CONTEXT_H

#include "PxPhysicsAPI.h"
//#include "Glen/Scene/Scene.h"
#include "Glen/Core/Logger.h"

#define PHYSX_SNIPPET_PVD_H
#define PVD_HOST "127.0.0.1"

#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

using namespace physx;

class RigidBody;

class PhysicsContext {
	friend class RigidBody;
private:
	PxDefaultCpuDispatcher* dispatcher;
	PxPvd* pvd;
	PxFoundation* foundation;
	PxDefaultAllocator memAllocator;

public:
	PhysicsContext() = default;
	void initialize();
	void release();

	PxMaterial* defaultMaterial;
	PxPhysics* physics;
	PxScene* physicsScene;
};

#endif // !PHYSICS_CONTEXT_H