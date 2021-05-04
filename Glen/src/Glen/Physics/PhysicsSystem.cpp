#include "PhysicsSystem.h"

void PhysicsSystem::startup()
{
	EngineContext::get()->physicsContext->initialize();
}

void PhysicsSystem::update(float deltaTime)
{
	EngineContext::get()->physicsContext->physicsScene->simulate(1.0f / 60.0f);
	EngineContext::get()->physicsContext->physicsScene->fetchResults(true);

	updateTransforms();
}



void PhysicsSystem::shutdown()
{
	EngineContext::get()->physicsContext->release();
}

void PhysicsSystem::updateTransforms()
{

	std::vector<Entity>& entities = EngineContext::get()->sceneManager->getEntities();
	for(int i = 0 ; i <entities.size(); i++ ){
		Entity& ent = entities[i];

		if (ent.rigidBody != nullptr) {
			PxRigidActor* rb = ent.rigidBody->getNativeRigidBody();
			PxShape* shapes[1];
			rb->getShapes(shapes, 1);
			PxTransform world = PxShapeExt::getGlobalPose(*shapes[0], *rb);

			glm::vec3 position(world.p.x, world.p.y, world.p.z);

			PxQuat q = world.q;

			double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
			double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
			double roll = std::atan2(sinr_cosp, cosr_cosp);

			double pitch;
			double sinp = 2 * (q.w * q.y - q.z * q.x);
			if (std::abs(sinp) >= 1)
				pitch = std::copysign(3.1415 / 2, sinp); // use 90 degrees if out of range
			else
				pitch = std::asin(sinp);

			double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
			double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
			double yaw = std::atan2(siny_cosp, cosy_cosp);

			// Hacky fix to attach rigidbody at the center of the mesh
			ent.transfrom.setPosition(position);
			ent.transfrom.setRotation(glm::vec3(roll, pitch, yaw));
		}
	}
}
