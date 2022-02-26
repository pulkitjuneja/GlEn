#include "PhysicsSystem.h"

void PhysicsSystem::startup()
{
	EngineContext::get()->physicsContext->initialize();
	EngineContext::get()->physicsContext->actorEntitymap.reserve(MAXENTITIES);
}

void PhysicsSystem::update(float deltaTime)
{
	buildRigidBody();

	EngineContext::get()->physicsContext->physicsScene->simulate(1/60.0f);
	EngineContext::get()->physicsContext->physicsScene->fetchResults(true);

	updateTransforms();
}


void PhysicsSystem::shutdown()
{
	EngineContext::get()->physicsContext->release();
}

// TODO: do this using an event system
void PhysicsSystem::buildRigidBody()
{
	std::vector<Entity>& entities = EngineContext::get()->sceneManager->getEntities();
	for (int i = 0; i < entities.size(); i++) {
		Entity& ent = entities[i];
		if (ent.rigidBody.isAttached && ent.rigidBody.getNativeRigidBody() == nullptr) {
			glm::vec3 entityCenterPosition = ent.transfrom.getPosition();
			PxVec3 p(entityCenterPosition.x, entityCenterPosition.y, entityCenterPosition.z);
			glm::quat rotationQuat = glm::quat(ent.transfrom.getEulerAngles());
			PxQuat q(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w);
			PxTransform localTm(p, q);
			if (ent.rigidBody.type == RigidBodyType::Dynamic) {
				ent.rigidBody.actorRef = EngineContext::get()->physicsContext->physics->createRigidDynamic(localTm);
				PxRigidBodyExt::updateMassAndInertia(*(PxRigidDynamic*)ent.rigidBody.actorRef, 100.0f); // TODO: make this controllable from the rb component
			}
			else {
				ent.rigidBody.actorRef = EngineContext::get()->physicsContext->physics->createRigidStatic(localTm);
			}
			PxShape* shape;
			shape = EngineContext::get()->physicsContext->physics->createShape(*ent.collider->colliderGeometry, *ent.collider->material);

			PxFilterData filterData;

			// Only one otype of object in collision filtering for now
			filterData.word0 = 1;
			filterData.word1 = 1;

			shape->setSimulationFilterData(filterData);

			ent.rigidBody.actorRef->attachShape(*shape);

			EngineContext::get()->physicsContext->physicsScene->addActor(*ent.rigidBody.actorRef);
			EngineContext::get()->physicsContext->actorEntitymap.insert(std::make_pair(ent.rigidBody.getNativeRigidBody(), &ent));
		}
	}
}

void PhysicsSystem::updateTransforms()
{

	std::vector<Entity>& entities = EngineContext::get()->sceneManager->getEntities();
	for(int i = 0 ; i <entities.size(); i++ ){
		Entity& ent = entities[i];

		if (ent.rigidBody.isAttached) {
			PxRigidActor* rb = ent.rigidBody.getNativeRigidBody();
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

			ent.transfrom.setPosition(position);
			ent.transfrom.setRotation(glm::vec3(roll, pitch, yaw));
		}
	}
}