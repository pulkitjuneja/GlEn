#pragma once

#ifndef PHYSICS_EVENTS_CALLBACK
#define PHYSICS_EVENTS_CALLBACK

#include "PxPhysicsAPI.h"
#include "Glen/Core/Logger.h"
#include "Glen/Core/EngineContext.h"

using namespace physx;

class PhysicsEventsCallbacks : public PxSimulationEventCallback {

	// Inherited via PxSimulationEventCallback
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
	virtual void onWake(PxActor** actors, PxU32 count) override;
	virtual void onSleep(PxActor** actors, PxU32 count) override;
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;
};

#endif // !PHYSICS_EVENTS_CALLBACK
