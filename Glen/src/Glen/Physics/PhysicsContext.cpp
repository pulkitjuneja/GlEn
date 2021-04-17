#include "PhysicsContext.h"

PhysicsContext* PhysicsContext::instance;

PhysicsContext* PhysicsContext::get()
{
	if (instance == nullptr)
	{
		instance = new PhysicsContext();
	}
	return instance;
}

void PhysicsContext::update()
{
	physicsScene->simulate(1.0f / 60.0f);
	physicsScene->fetchResults(true);
}

PhysicsContext::PhysicsContext()
{
	PxDefaultErrorCallback	ErrorCallback;
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, memAllocator, ErrorCallback);

	PxPvd* pvd = PxCreatePvd(*foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, pvd);

	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -0.081f, 0.0f);
    dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	physicsScene = physics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = physicsScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	defaultMaterial = physics->createMaterial(0.5f, 0.5f, 0.6f);
}

void PhysicsContext::shutdown()
{
	PX_RELEASE(physicsScene);
	PX_RELEASE(dispatcher);
	PX_RELEASE(physics);
	if (pvd)
	{
		PxPvdTransport* transport = pvd->getTransport();
		pvd->release();
		pvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(foundation);
}
