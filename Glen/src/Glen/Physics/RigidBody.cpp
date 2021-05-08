#include "RigidBody.h"

PxRigidActor* RigidBody::getNativeRigidBody()
{
	return actorRef;
}

void RigidBody::addForce(glm::vec3 force)
{
	if (type == RigidBodyType::Static) {
		Logger::logWarn("Cannot add force to a static rigidBody");
	}
	else {
		((PxRigidDynamic*)actorRef)->addForce(PxVec3(force.x, force.y, force.z));
	}
}

RigidBody::RigidBody()
{
	actorRef = nullptr;
}
