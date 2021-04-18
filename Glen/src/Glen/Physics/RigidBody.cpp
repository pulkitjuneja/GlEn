#include "RigidBody.h"

PxRigidActor* RigidBody::getNativeRigidBody()
{
	return rigidBody;
}

void RigidBody::addForce(glm::vec3 force)
{
	if (type == RigidBodyType::Static) {
		Logger::logWarn("Cannot add force to a static rigidBody");
	}
	else {
		((PxRigidDynamic*)rigidBody)->addForce(PxVec3(force.x, force.y, force.z));
	}
}

RigidBody::RigidBody(Transform& transform, Collider* collider, RigidBodyType type)
{	
	glm::vec3 centerPosition = transform.getPosition();
	PxVec3 p(centerPosition.x, centerPosition.y, centerPosition.z);
	glm::quat rotationQuat = glm::quat(transform.getEulerAngles());
	PxQuat q(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w);
	PxTransform localTm(p, q);
	if (type == RigidBodyType::Dynamic) {
		rigidBody = PhysicsContext::get()->getPhysicsRef()->createRigidDynamic(localTm);
		PxRigidBodyExt::updateMassAndInertia(*(PxRigidDynamic*)rigidBody, 10.0f);
	}
	else {
		rigidBody = PhysicsContext::get()->getPhysicsRef()->createRigidStatic(localTm);
	}
	PxShape* shape;
	shape = PhysicsContext::get()->getPhysicsRef()->createShape(*collider->colliderGeometry, *collider->material);
	rigidBody->attachShape(*shape);
	PhysicsContext::get()->getSceneRef()->addActor(*rigidBody);

	this->type = type;
}
