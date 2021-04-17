#include "RigidBody.h"

RigidBody::RigidBody(Transform& transform, Collider* collider)
{
	PxVec3 p(transform.getPosition().x, transform.getPosition().y, transform.getPosition().z);
	glm::quat rotationQuat = glm::quat(transform.getEulerAngles());
	PxQuat q(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w);
	PxTransform localTm(p,q);
	dynamic = PhysicsContext::get()->physics->createRigidDynamic(localTm);

	PxShape* shape;
	if (dynamic_cast<BoxCollider*>(collider)) {
		glm::vec3 halfExtents = dynamic_cast<BoxCollider*>(collider)->halfExtents;
		shape = PhysicsContext::get()->physics->createShape(PxBoxGeometry(halfExtents.x, halfExtents.y, halfExtents.z), 
			*collider->material);
	}
	else if (dynamic_cast<SphereCollider*>(collider)) {
		float radius = dynamic_cast<SphereCollider*>(collider)->radius;
		shape = PhysicsContext::get()->physics->createShape(PxSphereGeometry(radius),
			*collider->material);
	}
	dynamic->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*dynamic, 10.0f);
	PhysicsContext::get()->physicsScene->addActor(*dynamic);
}

PxRigidDynamic* RigidBody::getNativeRigidBody()
{
	return dynamic;
}
