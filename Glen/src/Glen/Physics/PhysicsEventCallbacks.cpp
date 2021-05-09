#include "PhysicsEventCallbacks.h"
#include "PhysicsContext.h"
#include "Glen/Scene/Entity.h"

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
		PxActor* rb1 = pairHeader.actors[0];
		PxActor* rb2 = pairHeader.actors[1];

		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
			EngineContext::get()->physicsContext->actorEntitymap[rb1] ->color = glm::vec3(1, 0, 0);
			EngineContext::get()->physicsContext->actorEntitymap[rb2]->color = glm::vec3(1, 0, 0);
		}
		else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {
			EngineContext::get()->physicsContext->actorEntitymap[rb1]->color = glm::vec3(1, 1, 1);
			EngineContext::get()->physicsContext->actorEntitymap[rb2]->color = glm::vec3(1, 1, 1);
		}
	}
}

void PhysicsEventsCallbacks::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
}

void PhysicsEventsCallbacks::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}
