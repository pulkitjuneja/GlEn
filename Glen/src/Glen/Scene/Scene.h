#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>
#include "Entity.h"
#include "Glen/Rendering/Light.h"
#include "Camera.h"

#define MAXENTITIES 5000
#define MAXPOINTLIGHTS 8192

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
	glm::mat4 VPPrevNoJitter;
	glm::mat4 VPPrevJittered;

	Entity& createEntity(std::string name, Mesh* mesh = nullptr, Material* overrideMat = nullptr);
	PointLight& createPointLight(glm::vec4 position, glm::vec4 diffuse, float intensity);
	DirectionalLight& createDirectionalLight(glm::vec4 direction, glm::vec4 diffuse, float intensity);
	void setMainCamera(glm::vec3 position, glm::vec3 front, float fov, float aspect, float near, float far);

	std::vector<Entity>& getEntities();
	std::vector<PointLight>& getPointLIghts();
	DirectionalLight& getDirectionalLight();
	Camera* getMainCamera();

	void Release();
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
