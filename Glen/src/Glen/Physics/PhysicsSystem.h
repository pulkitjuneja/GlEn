#pragma once 

#ifndef PHYSICS_SYSTEM

#include "Glen/Core/System.h"
#include "PhysicsContext.h"
#include "Glen/Core/EngineContext.h"
#include "Glen/Scene/Scene.h"
#include "Glen/Globals.h"

class PhysicsSystem : public ISystem {
public:
	virtual void startup() override;
	virtual void update(float deltaTime) override;
	virtual void shutdown() override;

	void buildRigidBody();
	void updateTransforms();
};

#endif // !PHYSICS_SYSTEM
