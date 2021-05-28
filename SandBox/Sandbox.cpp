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
		parent->transfrom.rotate(glm::vec3(0, 2 * deltaTime, 0));
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

		Entity& pbrSphere = scene->createEntity("PBRSphere", EngineContext::get()->resourceManager->loadMesh(pbrSphereLoc));

		// because the sphere fbx model does not export textures or because my importer does not work with fbx models , IDK 
		Texture2D* PbrSphereDiffuse = EngineContext::get()->resourceManager->loadTexture("sphere_Base_Color.png", "Assets/Meshes/PBRSphere/source", TextureType::DIFFUSE);
		Texture2D* PbrSphereMetallic = EngineContext::get()->resourceManager->loadTexture("sphere_Metallic.png", "Assets/Meshes/PBRSphere/source", TextureType::METALLIC);
		Texture2D* PbrSphereAO = EngineContext::get()->resourceManager->loadTexture("sphere_Mixed_AO.png", "Assets/Meshes/PBRSphere/source", TextureType::OCLUSSION);
		Texture2D* PbrSphereNormal = EngineContext::get()->resourceManager->loadTexture("sphere_Normal.png", "Assets/Meshes/PBRSphere/source", TextureType::NORMAL);
		Texture2D* PbrSphereRoughness = EngineContext::get()->resourceManager->loadTexture("sphere_Roughness.png", "Assets/Meshes/PBRSphere/source", TextureType::ROUGHNESS);
		Material& mat = pbrSphere.mesh->subMeshes[0].material;
		mat.diffuseMap = PbrSphereDiffuse;
		mat.MetallicMap = PbrSphereMetallic;
		mat.OclussionMap = PbrSphereAO;
		mat.RoughnessMap = PbrSphereRoughness;
		mat.normalMap = PbrSphereNormal;	

		Entity& crysisEntity = scene->createEntity("Crysis Entity", EngineContext::get()->resourceManager->loadMesh(crysisMeshLocation));
		Entity& sponzaEntity = scene->createEntity("Sponza Entity", EngineContext::get()->resourceManager->loadMesh(sponzaMeshLocation));
		Time testTime = timer.restart();
		Logger::logInfo("Resource load time" + std::to_string(testTime.getAsMilliSeconds()));

		Entity& groundPLaneEntity = scene->createEntity("Ground Plane");
		groundPLaneEntity.attachBoxCollider(glm::vec3(100, 1, 100));
		groundPLaneEntity.attachRigidBody(RigidBodyType::Static);

		sponzaEntity.transfrom.setScale(glm::vec3(0.3, 0.3, 0.3));
		crysisEntity.transfrom.setScale(glm::vec3(2, 2, 2));
		crysisEntity.attachCollider(ColliderType::Box);
		crysisEntity.transfrom.setPosition(glm::vec3(0, 80, 0));
		crysisEntity.attachRigidBody(RigidBodyType::Dynamic);

		pbrSphere.transfrom.setScale(glm::vec3(10, 10, 10));
		pbrSphere.transfrom.setPosition(glm::vec3(0, 10, 0));
		//pbrSphere.attachCollider(ColliderType::Box);

		std::vector<std::string> facePaths = {
			"right.jpg",
			"left.jpg",
			"top.jpg",
			"bottom.jpg",
			"front.jpg",
			"back.jpg"
		};

		CubeMap* skybox = EngineContext::get()->resourceManager->loadCubeMap(facePaths, "Assets/Textures/skybox");

		scene->setMainCamera(glm::vec3(-4.31142f, 55.923f, 191.538f), glm::vec3(-16.8f, -89.1506f, 0), 90.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 8.0f, 1000.0f);
		scene->createPointLight(glm::vec4(-20, 10, 5, 1.0), glm::vec4(2, 0, 0, 1.0), 1.0f);
		scene->createPointLight(glm::vec4(20, 10, 5, 1.0), glm::vec4(0, 2, 0, 1.0), 1.0f);
		//scene->createPointLight(glm::vec4(-10, 10, 80, 1.0), glm::vec4(0.01, 0.5f, 0.01, 1.0), glm::vec4(2, 2, 2, 1.0), glm::vec4(0.1, 0.1, 0.1, 1.0));
		scene->createPointLight(glm::vec4(-10, 10, -80, 1.0), glm::vec4(0, 2, 0, 1.0), 1.0f);
		scene->createPointLight(glm::vec4(300, 10, 5, 1.0), glm::vec4(0, 2, 0, 1.0), 1.0f);
		scene->createPointLight(glm::vec4(-300, 10, -10, 1.0), glm::vec4(0, 0, 2, 1.0), 1.0f);
		scene->createPointLight(glm::vec4(-300, 10, 10, 1.0), glm::vec4(2, 0, 0, 1.0), 1.0f);
		scene->createDirectionalLight(glm::vec4(0.5f, -1.0f, 0, 1.0),  glm::vec4(3.8f, 3.8f, 3.8f, 1.0f), 1.0f);
		Entity& cameraControllerParent = scene->createEntity("cameraController");
		cameraControllerParent.AddScript<CameraController>();
		crysisEntity.AddScript<Bouncer>();
		pbrSphere.AddScript<ObjectRotator>();
		return true;
	}

	void loadShaders() {
		EngineContext::get()->resourceManager->loadShader("Assets/Shaders/basic.vert", "Assets/Shaders/basic.frag", "defaultShader");
		EngineContext::get()->resourceManager->loadShader("Assets/Shaders/TexturedMeshUnlit.vert", "Assets/Shaders/TexturedMeshUnlit.frag", "texturedMeshUnlit");
		EngineContext::get()->resourceManager->loadShader("Assets/Shaders/TexturedMesh.vert", "Assets/Shaders/TexturedMesh.frag", "texturedMeshShader");
	}
};

void main() {
	TestWorld world;
	world.start();
}