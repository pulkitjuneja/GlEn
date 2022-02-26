#include "SceneRenderer.h"

void SceneRenderer::setGlobalUniforms(PerFrameUniforms& perFrameUniforms, SceneManager* scene)
{
	Camera* mainCamera = scene->getMainCamera();
	perFrameUniforms.viewMatrix = mainCamera->getViewMatrix();
	perFrameUniforms.projectionMatrix = mainCamera->getProjectionMatrix();
	perFrameUniforms.inverseViewMatrix = glm::inverse(perFrameUniforms.viewMatrix);
	perFrameUniforms.inverseProjectionMatrix = glm::inverse(perFrameUniforms.projectionMatrix);
	glm::vec3 cameraPosition = mainCamera->transform.getPosition();
	perFrameUniforms.cameraPosition = glm::vec4(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);

	// set directional light
	perFrameUniforms.directionalLight = scene->getDirectionalLight();

	// set pointlight(s) into shader
	auto pointLights = scene->getPointLIghts();
	perFrameUniforms.pointLightCount = pointLights.size();


}

void SceneRenderer::bindGlobalMaps()
{
	Texture* depthMapTexture = EngineContext::get()->resourceManager->getTexture(CSM_SHADOW_MAPS);
	if (depthMapTexture) {
		depthMapTexture->bind(GL_TEXTURE0 + 10);
	}
}

void SceneRenderer::renderScene(SceneManager* scene, Material* overrideMaterial, bool passBaseMaterialProperties)
{
	std::vector<Entity> entities = scene->getEntities();
	std::vector<Entity>::iterator it = entities.begin();


	for (; it != entities.end(); it++) {
		Entity& ent = (*it);

		if (ent.mesh == nullptr) {
			continue;
		}

		Mesh* currentMesh = ent.mesh;
		glBindVertexArray(currentMesh->VAO);

		glm::mat4 modelMatrix = ent.getTransform()->getTransformationMatrix();

		glm::mat4 prevModelMatrix = ent.prevModelMatrix;

		Shader* currentShader = nullptr;

		for (int i = 0; i < currentMesh->subMeshes.size(); i++) {

			SubMesh currentSubMesh = currentMesh->subMeshes[i];
			Shader* submeshShader = currentSubMesh.material.getShader();

			if (overrideMaterial) {
				currentShader = overrideMaterial->getShader();
			}
			else {
				currentShader = submeshShader;
			}

			currentShader->use();
			currentShader->setMat4("modelMatrix", modelMatrix);;
			currentShader->setMat4("prevModelMatrix", prevModelMatrix);;

			unsigned int hasSpecularMap = 0;
			unsigned int hasNormalMap = 0;
			int j = 0;
			if (!overrideMaterial || (overrideMaterial && passBaseMaterialProperties)) {
				if (currentSubMesh.material.diffuseMap != NULL) {
					currentSubMesh.material.diffuseMap->bind(GL_TEXTURE0 + 1);
					currentShader->setInt("material.texture_diffuse", 1);
				}
				if (currentSubMesh.material.specularMap != NULL) {
					currentSubMesh.material.specularMap->bind(GL_TEXTURE0 + 2);
					currentShader->setInt("material.texture_specular", 2);
					hasSpecularMap = 1;
				}
				if (currentSubMesh.material.normalMap != NULL) {
					currentSubMesh.material.normalMap->bind(GL_TEXTURE0 + 3);
					currentShader->setInt("material.texture_normal", 3);
					hasNormalMap = 1;
				}
				if (currentSubMesh.material.MetallicMap != NULL) {
					currentSubMesh.material.MetallicMap->bind(GL_TEXTURE0 + 4);
					currentShader->setInt("material.texture_metallic", 4);
					currentShader->setFloat("material.usePBRWorkflow", 1.0f);
				}
				else {
					currentShader->setFloat("material.usePBRWorkflow", 0.0f);
				}
				if (currentSubMesh.material.RoughnessMap != NULL) {
					currentSubMesh.material.RoughnessMap->bind(GL_TEXTURE0 + 5);
					currentShader->setInt("material.texture_roughness", 5);
				}
				if (currentSubMesh.material.OclussionMap != NULL) {
					currentSubMesh.material.OclussionMap->bind(GL_TEXTURE0 + 6);
					currentShader->setInt("material.texture_occlusion", 6);
				}
				currentShader->setInt("material.hasNormalMap", hasNormalMap);
				currentShader->setInt("material.hasSpecularMap", hasSpecularMap);
			}
			currentShader->setInt("shadowMap", 10);
			glDrawElementsBaseVertex(GL_TRIANGLES, currentSubMesh.indexCount, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * currentSubMesh.baseIndex), currentSubMesh.baseVertex);

		}
	}
	glBindVertexArray(0);
}
