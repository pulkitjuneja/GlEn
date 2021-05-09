#include "PhysicsEventCallbacks.h"

void PhysicsEventsCallbacks::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
}

void PhysicsEventsCallbacks::onWake(PxActor** actors, PxU32 count)
{
}

void PhysicsEventsCallbacks::onSleep(PxActor** actors, PxU32 count)
{
}

void PhysicsEventsCallbacks::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (int i = 0; i < nbPairs; i++)
	{	
		const PxContactPair& cp = pairs[i];
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			Logger::logInfo("Coollision touch");
		}
	}
}

void PhysicsEventsCallbacks::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
}

void PhysicsEventsCallbacks::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}
