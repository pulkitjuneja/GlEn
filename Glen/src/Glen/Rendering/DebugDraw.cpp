#include "DebugDraw.h"
#include "Glen/Scene/Scene.h"

void DebugDraw::startup()
{
	DebugCubeMesh = EngineContext::get()->resourceManager->getMesh("CUBE");
	this->scene = EngineContext::get()->sceneManager;
}

void DebugDraw::update(float deltaTime) {
	std::vector<Entity> entities = scene->getEntities();
	std::vector<Entity>::iterator it = entities.begin();
	
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (; it != entities.end(); it++) {
		
		Entity& ent = (*it);

		if (ent.collider == nullptr) {
			continue;
		}

		Shader* basicShader = EngineContext::get()->resourceManager->getShader("texturedMeshUnlit");
		basicShader->use();
		//basicShader->setInt("skybox", 16);
		
		// TODO : add debug draw fro sphere colliders
		BoxCollider* boxCollider = (BoxCollider*)ent.collider;

		glm::mat4 transformationMAtrix(1.0f);
		glm::vec3 scale = boxCollider->halfExtents;
		glm::quat rotationQuat = glm::quat(ent.transfrom.getEulerAngles());
		glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);
		transformationMAtrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, scale.z)) * transformationMAtrix;
		transformationMAtrix = rotationMatrix * transformationMAtrix;
		glm::vec3 meshCenter = ent.mesh != nullptr? ent.collider->positionOffset: glm::vec3(0, 0, 0);
		transformationMAtrix = glm::translate(glm::mat4(1.0f), ent.transfrom.getPosition()) * transformationMAtrix;
		basicShader->setFloat3("debugColor", ent.color.x, ent.color.y, ent.color.z);
		basicShader->setMat4("modelMatrix", transformationMAtrix);


		glBindVertexArray(DebugCubeMesh->VAO);
		SubMesh submesh = DebugCubeMesh->subMeshes[0];
		glDrawElementsBaseVertex(GL_TRIANGLES, submesh.indexCount, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * submesh.baseIndex), submesh.baseVertex);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

void DebugDraw::shutdown() {}
