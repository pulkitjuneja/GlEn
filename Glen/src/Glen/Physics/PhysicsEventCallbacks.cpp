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

	// TODO: Do this using event system
	for (int i = 0; i < nbPairs; i++)
	{	
		const PxContactPair& cp = pairs[i];
		PxActor* rb1 = pairHeader.actors[0];
		PxActor* rb2 = pairHeader.actors[1];

		Entity* ent1 = EngineContext::get()->physicsContext->actorEntitymap[rb1];
		Entity* ent2 = EngineContext::get()->physicsContext->actorEntitymap[rb2];

		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
			for (Script* script : ent1->scripts) {
				script->onCollisionBegin(*ent2);
			}

			for (Script* script : ent2->scripts) {
				script->onCollisionBegin(*ent1);
			}
		}
		else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {

			for (Script* script : ent1->scripts) {
				script->onCollisionEnd(*ent2);
			}

			for (Script* script : ent2->scripts) {
				script->onCollisionEnd(*ent1);
			}
		}
	}
}

void PhysicsEventsCallbacks::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
}

void PhysicsEventsCallbacks::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}
