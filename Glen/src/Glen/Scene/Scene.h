#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>
#include "Entity.h"
#include "Glen/Rendering/Light.h"
#include "Camera.h"

#define MAXENTITIES 5000
#define MAXPOINTLIGHTS 1000

class GLN_API SceneManager {
protected:
	std::vector<Entity> Entities;
	std::vector<PointLight> pointLights;
	DirectionalLight directionalLight;
	Camera* mainCamera;

public:

	SceneManager();

	//TODO: remove this implmenet uniform buffers to fix uniform assignement;
	glm::mat4 directionalLightSpaceMatrix;

	Entity& createEntity(std::string name, Mesh* mesh = nullptr, Material* overrideMat = nullptr);
	PointLight& createPointLight(glm::vec4 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
	DirectionalLight& createDirectionalLight(glm::vec4 direction, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
	void setMainCamera(Camera* camera);

	std::vector<Entity>& getEntities();
	std::vector<PointLight>& getPointLIghts();
	DirectionalLight& getDirectionalLight();
	Camera* getMainCamera();
};

inline SceneManager::SceneManager()
{
	Entities.reserve(MAXENTITIES);
	pointLights.reserve(MAXPOINTLIGHTS);
}

inline Entity& SceneManager::createEntity(std::string name, Mesh* mesh, Material* overrideMat)
{
	Entity ent(name);
	ent.mesh = mesh;
	ent.overrideMaterial = overrideMat;

	Entities.push_back(ent);
	return Entities[Entities.size() - 1];
}

#endif
