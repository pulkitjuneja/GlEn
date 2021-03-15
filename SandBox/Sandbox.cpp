#include "Glen.h"

class TestWorld : public Engine {
	Entity* cubeEntity;
public:
	TestWorld() {};
	bool init() {
		loadShaders();
		scene->setMainCamera(new Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, -90.0f, 0.0f), 90.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 8.0f, 1000.0f));
		scene->createDirectionalLight(glm::vec4(0.5f, -1.0f, 0, 1.0), glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(3.8f, 3.8f, 3.8f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
		Mesh* cubeMesh = createCubeMesh();
		cubeEntity = scene->createEntity<Entity>("cube", cubeMesh);

		return true;
	}

	Mesh* createCubeMesh() {
		std::vector<Vertex> vertices = {
			Vertex(glm::vec3(0,1,0), glm::vec3(0,0,0), glm::vec2(0,0.66)),
			Vertex(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec2(0.25f, 0.66f)),
			Vertex(glm::vec3(1,1,0), glm::vec3(0,0,0), glm::vec2(0.0f, 0.33f)),
			Vertex(glm::vec3(1,0,0), glm::vec3(0,0,0), glm::vec2(0.25f, 0.66f)),

			Vertex(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec2(0.5f, 0.66f)),
			Vertex(glm::vec3(1,0,1), glm::vec3(0,0,0), glm::vec2(0.5f, 0.33f)),
			Vertex(glm::vec3(0,1,1), glm::vec3(0,0,0), glm::vec2(0.75f, 0.66f)),
			Vertex(glm::vec3(1,1,1), glm::vec3(0,0,0), glm::vec2(0.75f, 0.33f)),

			Vertex(glm::vec3(0,1,0), glm::vec3(0,0,0), glm::vec2(1.0f, 0.66f)),
			Vertex(glm::vec3(1,1,0), glm::vec3(0,0,0), glm::vec2(1.0f, 0.33f)),

			Vertex(glm::vec3(0,1,0), glm::vec3(0,0,0), glm::vec2(0.25f, 1.0f)),
			Vertex(glm::vec3(0,1,1), glm::vec3(0,0,0), glm::vec2(0.5f, 1.0f)),

			Vertex(glm::vec3(1,1,0), glm::vec3(0,0,0), glm::vec2(0.25f, 0.0f)),
			Vertex(glm::vec3(1,1,1), glm::vec3(0,0,0), glm::vec2(0.5f, 0.0f)),
		};

		std::vector<unsigned int> indices = {
			0, 2, 1, // front
			1, 2, 3,
			4, 5, 6, // back
			5, 7, 6,
			6, 7, 8, //top
			7, 9 ,8,
			1, 3, 4, //bottom
			3, 5, 4,
			1, 11,10,// left
			1, 4, 11,
			3, 12, 5,//right
			5, 12, 13
		};

		Material* cubeMaterial = new Material();
		cubeMaterial->setShader(ResourceManager::getInstance()->getShader("texturedMeshUnlit"));
		cubeMaterial->diffuseMap = ResourceManager::getInstance()->loadTexture("Assets/Textures/crate_1.jpg", ".", TextureType::DIFFUSE);

		std::vector<SubMesh> cubeSubmeshes = {
			SubMesh(cubeMaterial,0,36,0)
		};

		Mesh* cubeMesh = new Mesh(vertices, indices, cubeSubmeshes, true, true);
		return cubeMesh;
	}

	void loadShaders() {
		ResourceManager::getInstance()->loadShader("Assets/Shaders/basic.vert", "Assets/Shaders/basic.frag", "defaultShader");
		ResourceManager::getInstance()->loadShader("Assets/Shaders/TexturedMeshUnlit.vert", "Assets/Shaders/TexturedMeshUnlit.frag", "texturedMeshUnlit");
		ResourceManager::getInstance()->loadShader("Assets/Shaders/TexturedMesh.vert", "Assets/Shaders/TexturedMesh.frag", "texturedMeshShader");
	}

	void update() {
		cubeEntity->transfrom.rotate(glm::vec3(0, 0.005, 0));
		scene->update();
	}
};


int main() {
	TestWorld* world = new TestWorld();
	world->start();
}