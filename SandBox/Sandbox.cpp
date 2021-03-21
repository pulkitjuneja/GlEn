#include "Glen.h"
#include "CameraController.h";


class TestWorld : public Engine {
	Entity* sponzaEntity;
	CameraController* cameraController;
public:
	TestWorld() {};
	bool init() {
		loadShaders();
		string sponzaMeshLocation = "Assets/Meshes/Sponza/sponza.obj";

		sponzaEntity = scene->createEntity<Entity>("Model", ResourceManager::getInstance()->loadMesh(sponzaMeshLocation));

		sponzaEntity->transfrom.setScale(glm::vec3(0.3, 0.3, 0.3));

		scene->setMainCamera(new Camera(glm::vec3(-4.31142f, 55.923f, 191.538f), glm::vec3(-16.8f, -89.1506f, 0), 90.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 8.0f, 1000.0f));
		scene->createPointLight(glm::vec4(-20, 10, 5, 1.0), glm::vec4(0.3f, 0.01, 0.01, 1.0), glm::vec4(0.8, 0, 0, 1.0), glm::vec4(1, 0, 0, 1.0));
		scene->createPointLight(glm::vec4(20, 10, 5, 1.0), glm::vec4(0.3f, 0.01, 0.01, 1.0), glm::vec4(0, 0.8, 0, 1.0), glm::vec4(0, 1, 0, 1.0));
		scene->createPointLight(glm::vec4(-10, 10, 80, 1.0), glm::vec4(0.01, 0.5f, 0.01, 1.0), glm::vec4(0.8, 0.8, 0.8, 1.0), glm::vec4(0.1, 0.1, 0.1, 1.0));
		scene->createPointLight(glm::vec4(-10, 10, -80, 1.0), glm::vec4(0.01, 0.01, 0.5, 1.0), glm::vec4(0, 0.8, 0, 1.0), glm::vec4(0.1, 0.1, 0.1, 1.0));
		scene->createPointLight(glm::vec4(300, 10, 5, 1.0), glm::vec4(0.01, 0.01, 0.3f, 1.0), glm::vec4(0, 0.8, 0, 1.0), glm::vec4(0, 1, 0, 1.0));
		scene->createPointLight(glm::vec4(-300, 10, 5, 1.0), glm::vec4(0.01, 0.01, 0.01, 1.0), glm::vec4(0, 0, 0.8, 1.0), glm::vec4(5, 0, 1, 1.0));
		scene->createDirectionalLight(glm::vec4(0.5f, -1.0f, 0, 1.0), glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
		cameraController = new CameraController(scene->getMainCamera());


		return true;
	}

	void loadShaders() {
		ResourceManager::getInstance()->loadShader("Assets/Shaders/basic.vert", "Assets/Shaders/basic.frag", "defaultShader");
		ResourceManager::getInstance()->loadShader("Assets/Shaders/TexturedMeshUnlit.vert", "Assets/Shaders/TexturedMeshUnlit.frag", "texturedMeshUnlit");
		ResourceManager::getInstance()->loadShader("Assets/Shaders/TexturedMesh.vert", "Assets/Shaders/TexturedMesh.frag", "texturedMeshShader");
	}

	void update() {
		cameraController->update();
		scene->update();
	}
};


int main() {
	TestWorld* world = new TestWorld();
	world->start();
}