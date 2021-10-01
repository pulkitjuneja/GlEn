#include "Glen.h"
#include "src/CameraController.h"
#include "PxPhysicsAPI.h"


class Bouncer : public Script {

	void Update(float deltaTime) override {
		if (EngineContext::get()->inputStatus->isKeyPressed(Keys::Space)) {
			parent->rigidBody.addForce(glm::vec3(0, 50, 0));
		}
	}

	void onCollisionBegin(Entity& other) {
		parent->color = glm::vec3(1, 0, 0);
	}

	void onCollisionEnd(Entity& other) {
		parent->color = glm::vec3(1, 1, 1);
	}
};

class ObjectRotator : public Script {

	void Update(float deltaTime) override {
		parent->transfrom.rotate(glm::vec3(0, 5.6 * deltaTime, 0));
	}
};


class TestWorld : public Engine {
public:
	TestWorld() {};
	bool init() {
		Timer timer;
		loadShaders();
		std::string sponzaMeshLocation = "Assets/Meshes/Sponza/sponza.obj";
		std::string crysisMeshLocation = "Assets/Meshes/crysisM/nanosuit.obj";
		std::string pbrSphereLoc = "Assets/Meshes/PBRSphere/source/sphere_sampleScene_1cm.fbx";
		std::string pbrSphereLoc2 = "Assets/Meshes/PBRSphere/source/sphere_sampleScene_1cm-2.fbx";
		std::string pbrSphereLoc3 = "Assets/Meshes/PBRSphere/source/sphere_sampleScene_1cm-3.fbx";

		Entity& pbrSphereGold = scene->createEntity("PBRSphereGold", EngineContext::get()->resourceManager->loadMesh(pbrSphereLoc));
		Entity& pbrSphereIron = scene->createEntity("PBRSphereIron", EngineContext::get()->resourceManager->loadMesh(pbrSphereLoc2));
		Entity& pbrSpherePlastic = scene->createEntity("PBRSpherePlastic", EngineContext::get()->resourceManager->loadMesh(pbrSphereLoc3));

		// because the sphere fbx model does not export textures or because my importer does not work with fbx models , IDK 
		Material& mat = pbrSphereGold.mesh->subMeshes[0].material;
		mat.diffuseMap = EngineContext::get()->resourceManager->loadTexture("gold_albedo.png", "Assets/Meshes/PBRSphere/source", TextureType::DIFFUSE);
		mat.MetallicMap = EngineContext::get()->resourceManager->loadTexture("gold_Metallic.png", "Assets/Meshes/PBRSphere/source", TextureType::METALLIC);
		mat.OclussionMap = EngineContext::get()->resourceManager->loadTexture("sphere_Mixed_AO.png", "Assets/Meshes/PBRSphere/source", TextureType::OCLUSSION);
		mat.RoughnessMap = EngineContext::get()->resourceManager->loadTexture("gold_roughness.png", "Assets/Meshes/PBRSphere/source", TextureType::ROUGHNESS);
		mat.normalMap = EngineContext::get()->resourceManager->loadTexture("sphere_Normal.png", "Assets/Meshes/PBRSphere/source", TextureType::NORMAL);
		mat.setShader(EngineContext::get()->resourceManager->getShader("texturedMeshPBR"));

		Material& mat2 = pbrSphereIron.mesh->subMeshes[0].material;
		mat2.diffuseMap = EngineContext::get()->resourceManager->loadTexture("iron_albedo.png", "Assets/Meshes/PBRSphere/source", TextureType::DIFFUSE);
		mat2.MetallicMap = EngineContext::get()->resourceManager->loadTexture("iron_Metallic.png", "Assets/Meshes/PBRSphere/source", TextureType::METALLIC);
		mat2.OclussionMap = EngineContext::get()->resourceManager->loadTexture("sphere_Mixed_AO.png", "Assets/Meshes/PBRSphere/source", TextureType::OCLUSSION);
		mat2.RoughnessMap = EngineContext::get()->resourceManager->loadTexture("iron_roughness.png", "Assets/Meshes/PBRSphere/source", TextureType::ROUGHNESS);;
		mat2.normalMap = EngineContext::get()->resourceManager->loadTexture("sphere_Normal.png", "Assets/Meshes/PBRSphere/source", TextureType::NORMAL);
		mat2.setShader(EngineContext::get()->resourceManager->getShader("texturedMeshPBR"));

		Material& mat3 = pbrSpherePlastic.mesh->subMeshes[0].material;
		mat3.diffuseMap = EngineContext::get()->resourceManager->loadTexture("plastic_albedo.png", "Assets/Meshes/PBRSphere/source", TextureType::DIFFUSE);
		mat3.MetallicMap = EngineContext::get()->resourceManager->loadTexture("plastic_Metallic.png", "Assets/Meshes/PBRSphere/source", TextureType::METALLIC);
		mat3.OclussionMap = EngineContext::get()->resourceManager->loadTexture("sphere_Mixed_AO.png", "Assets/Meshes/PBRSphere/source", TextureType::OCLUSSION);
		mat3.RoughnessMap = EngineContext::get()->resourceManager->loadTexture("plastic_roughness.png", "Assets/Meshes/PBRSphere/source", TextureType::ROUGHNESS);;
		mat3.normalMap = EngineContext::get()->resourceManager->loadTexture("sphere_Normal.png", "Assets/Meshes/PBRSphere/source", TextureType::NORMAL);
		mat3.setShader(EngineContext::get()->resourceManager->getShader("texturedMeshPBR"));

		//Entity& crysisEntity = scene->createEntity("Crysis Entity", EngineContext::get()->resourceManager->loadMesh(crysisMeshLocation));
		Entity& sponzaEntity = scene->createEntity("Sponza Entity", EngineContext::get()->resourceManager->loadMesh(sponzaMeshLocation));
		Time testTime = timer.restart();
		Logger::logInfo("Resource load time" + std::to_string(testTime.getAsMilliSeconds()));

		//Entity& groundPLaneEntity = scene->createEntity("Ground Plane");
		//groundPLaneEntity.attachBoxCollider(glm::vec3(100, 1, 100));
		//groundPLaneEntity.attachRigidBody(RigidBodyType::Static);

		sponzaEntity.transfrom.setScale(glm::vec3(0.3, 0.3, 0.3));
		//crysisEntity.transfrom.setScale(glm::vec3(2, 2, 2));
		//crysisEntity.attachCollider(ColliderType::Box);
		//crysisEntity.transfrom.setPosition(glm::vec3(0, 80, 0));
		//crysisEntity.attachRigidBody(RigidBodyType::Dynamic);

		pbrSphereGold.transfrom.setScale(glm::vec3(10, 10, 10));
		pbrSphereGold.transfrom.setPosition(glm::vec3(0, 10, 0));
		pbrSphereIron.transfrom.setScale(glm::vec3(10, 10, 10));
		pbrSphereIron.transfrom.setPosition(glm::vec3(15, 10, 0));
		pbrSpherePlastic.transfrom.setScale(glm::vec3(10, 10, 10));
		pbrSpherePlastic.transfrom.setPosition(glm::vec3(30, 10, 0));
		//pbrSphere.attachCollider(ColliderType::Box);

		scene->setMainCamera(glm::vec3(-4.31142f, 55.923f, 191.538f), glm::vec3(-16.8f, -89.1506f, 0), 90.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 8.0f, 1000.0f);
		scene->createPointLight(glm::vec4(-20, 10, 5, 1.0), glm::vec4(1, 0, 0, 1.0), 1.0f);
		scene->createPointLight(glm::vec4(20, 10, 5, 1.0), glm::vec4(0, 1, 0, 1.0), 1.0f);
		//scene->createPointLight(glm::vec4(-10, 10, 80, 1.0), glm::vec4(0.01, 0.5f, 0.01, 1.0), glm::vec4(2, 2, 2, 1.0), glm::vec4(0.1, 0.1, 0.1, 1.0));
		scene->createPointLight(glm::vec4(-10, 10, -80, 1.0), glm::vec4(1, 1, 0, 1.0), 0.8f);
		scene->createPointLight(glm::vec4(300, 10, 5, 1.0), glm::vec4(1, 0	, 1, 1.0), 0.8f);
		scene->createPointLight(glm::vec4(-300, 10, -10, 1.0), glm::vec4(0, 1, 1, 1.0), 0.8f);
		scene->createPointLight(glm::vec4(-300, 10, 10, 1.0), glm::vec4(0, 1, 1, 1.0), 0.8f);
		//for (int i = 0; i < 8000; i++) {
		//	scene->createPointLight(glm::vec4((std::rand() - RAND_MAX/2) % 400, std::rand()% 300 + 10, (std::rand() - RAND_MAX / 2) %200, 1.0), glm::vec4((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0), 1.0f);
		//}
		scene->createDirectionalLight(glm::vec4(0.5f, -1.0f, 0, 1.0),  glm::vec4(3.8f, 3.8f, 3.8f, 1.0f), 1.1f);
		Entity& cameraControllerParent = scene->createEntity("cameraController");
		cameraControllerParent.AddScript<CameraController>();
		//crysisEntity.AddScript<Bouncer>();
		pbrSphereGold.AddScript<ObjectRotator>();

		return true;
	}

	void loadShaders() {
		EngineContext::get()->resourceManager->loadShader("Assets/Shaders/basic.vert", "Assets/Shaders/basic.frag", "defaultShader");
		EngineContext::get()->resourceManager->loadShader("Assets/Shaders/TexturedMeshUnlit.vert", "Assets/Shaders/TexturedMeshUnlit.frag", "texturedMeshUnlit");
		EngineContext::get()->resourceManager->loadShader("Assets/Shaders/TexturedMesh.vert", "Assets/Shaders/TexturedMesh.frag", "texturedMeshShader");
		EngineContext::get()->resourceManager->loadShader("Assets/Shaders/TexturedMesh.vert", "Assets/Shaders/TexturedMeshPBR.frag", "texturedMeshPBR");
	}
};

void main() {
	TestWorld world;
	world.start();
}