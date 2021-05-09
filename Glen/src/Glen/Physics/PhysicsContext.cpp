#include "PhysicsContext.h"



PxFilterFlags CollisionFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;

	return PxFilterFlag::eDEFAULT;
}

PhysicsContext::~PhysicsContext()
{
	this->release();
	Logger::logInfo("Physics released");
}

void PhysicsContext::initialize()
{
	PxDefaultErrorCallback	ErrorCallback;
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, memAllocator, ErrorCallback);

	PxPvd* pvd = PxCreatePvd(*foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, pvd);

	PhysicsEventsCallbacks* callbacks = Mem::Allocate<PhysicsEventsCallbacks>();

	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81, 0.0f);
	dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.simulationEventCallback = callbacks;
	sceneDesc.filterShader = CollisionFilterShader;
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

void PhysicsContext::release()
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
