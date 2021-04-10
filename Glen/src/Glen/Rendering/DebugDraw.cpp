#include "DebugDraw.h"
#include "Glen/Scene/Scene.h"

Mesh* DebugDraw::createCubeMesh()
{
	std::vector<Vertex> vertices = {
	Vertex(glm::vec3(-1,-1,-1), glm::vec3(0,0,0), glm::vec2(0,0.66)),
	Vertex(glm::vec3(1,-1,-1), glm::vec3(0,0,0), glm::vec2(0.25f, 0.66f)),
	Vertex(glm::vec3(1,1,-1), glm::vec3(0,0,0), glm::vec2(0.0f, 0.33f)),
	Vertex(glm::vec3(-1,1,-1), glm::vec3(0,0,0), glm::vec2(0.25f, 0.66f)),

	Vertex(glm::vec3(-1,-1,1), glm::vec3(0,0,0), glm::vec2(0.5f, 0.66f)),
	Vertex(glm::vec3(1,-1,1), glm::vec3(0,0,0), glm::vec2(0.5f, 0.33f)),
	Vertex(glm::vec3(1,1,1), glm::vec3(0,0,0), glm::vec2(0.75f, 0.66f)),
	Vertex(glm::vec3(-1,1,1), glm::vec3(0,0,0), glm::vec2(0.75f, 0.33f))
	};

	std::vector<unsigned int> indices = {
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};

	Material* cubeMaterial = new Material();
	cubeMaterial->setShader(ResourceManager::getInstance()->getShader("texturedMeshUnlit"));
	cubeMaterial->diffuseMap = ResourceManager::getInstance()->loadTexture("Assets/Textures/crate_1.jpg", ".", TextureType::DIFFUSE);

	std::vector<SubMesh> cubeSubmeshes = {
		SubMesh(cubeMaterial,0,36,0)
	};

	Mesh* cubeMesh = new Mesh(vertices, indices, cubeSubmeshes, false, false, false);
	return cubeMesh;
}

void DebugDraw::render(Scene* scene) {
	std::vector<Entity*> entities = scene->getEntities();
	std::vector<Entity*>::iterator it = entities.begin();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (; it != entities.end(); it++) {
		
		Entity* ent = (*it);

		if (ent->DebugMesh == nullptr) {
			continue;
		}

		Shader* basicShader = ResourceManager::getInstance()->getShader("texturedMeshUnlit");
		basicShader->use();

		glm::mat4 transformationMAtrix(1.0f);
		glm::vec3 scale = ent->mesh->bounds.getExtents() * ent->transfrom.getScale();
		glm::quat rotationQuat = glm::quat(ent->transfrom.getEulerAngles());
		glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);
		transformationMAtrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x / 2, scale.y / 2, scale.z / 2)) * transformationMAtrix;
		transformationMAtrix = rotationMatrix * transformationMAtrix;
		transformationMAtrix = glm::translate(glm::mat4(1.0f), ent->mesh->bounds.getCenter() * ent->transfrom.getScale() + ent->transfrom.getPosition()) * transformationMAtrix;
		basicShader->setMat4("modelMatrix", transformationMAtrix);

		glBindVertexArray(ent->DebugMesh->VAO);
		SubMesh submesh = ent->DebugMesh->subMeshes[0];
		glDrawElementsBaseVertex(GL_TRIANGLES, submesh.indexCount, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * submesh.baseIndex), submesh.baseVertex);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
