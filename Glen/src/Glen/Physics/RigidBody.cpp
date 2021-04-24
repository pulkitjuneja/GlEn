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
		rigidBody = EngineContext::get()->physicsContext->physics->createRigidDynamic(localTm);
		PxRigidBodyExt::updateMassAndInertia(*(PxRigidDynamic*)rigidBody, 10.0f);
	}
	else {
		rigidBody = EngineContext::get()->physicsContext->physics->createRigidStatic(localTm);
	}
	PxShape* shape;
	shape = EngineContext::get()->physicsContext->physics->createShape(*collider->colliderGeometry, *collider->material);
	rigidBody->attachShape(*shape);
	EngineContext::get()->physicsContext->physicsScene->addActor(*rigidBody);

	this->type = type;
}
