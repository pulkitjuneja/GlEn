#pragma once 

#ifndef PHYSICS_CONTEXT_H

#include "PxPhysicsAPI.h"
//#include "Glen/Scene/Scene.h"

#define PHYSX_SNIPPET_PVD_H
#define PVD_HOST "127.0.0.1"

#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

using namespace physx;

class RigidBody;

class PhysicsContext {
	friend class RigidBody;
private:
	PxPhysics* physics;
	PxScene* physicsScene;
	PxDefaultCpuDispatcher* dispatcher;
	PxPvd* pvd;
	PxFoundation* foundation;
	PxDefaultAllocator memAllocator;

	PhysicsContext();
	static PhysicsContext* instance;
public:
	PxMaterial* defaultMaterial;
	static PhysicsContext* get();
	PxPhysics* getPhysicsRef() { return physics; }
	PxScene* getSceneRef() { return physicsScene; }
	void update();
	void shutdown();
};

#endif // !PHYSICS_CONTEXT_H
