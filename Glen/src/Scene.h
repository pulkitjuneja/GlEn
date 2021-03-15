#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>
#include "Entity.h"
#include "Light.h"
#include "Camera.h"

// TODO : use this class to abstract out Entt from the user of the engine

class GLN_API Scene {
protected:
	std::vector<Entity*> Entities;
	std::vector<PointLight*> pointLights;
	DirectionalLight* directionalLight;
	Camera* mainCamera;
	//Mesh* cameraFrustumMesh;

public:

	//TODO: remove this implmenet uniform buffers to fix uniform assignement;
	glm::mat4 directionalLightSpaceMatrix;

	template <class T >
	T* createEntity(string name, Mesh* mesh = nullptr, Material* overrideMat = nullptr);
	PointLight* createPointLight(glm::vec4 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
	DirectionalLight* createDirectionalLight(glm::vec4 direction, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
	void setMainCamera(Camera* camera);

	std::vector<Entity*>& getEntities();
	std::vector<PointLight*>& getPointLIghts();
	DirectionalLight* getDirectionalLight();
	Camera* getMainCamera();

	virtual void update();
};

template<class T>
inline T* Scene::createEntity(string name, Mesh* mesh, Material* overrideMat)
{
	T* ent = new T(name);
	// this is to make sure T is always an entity
	static_cast<Entity*>(ent);
	if (mesh != nullptr) {
		ent->mesh = mesh;
	}
	if (overrideMat != nullptr) {
		ent->overrideMaterial = overrideMat;
	}

	Entities.push_back(ent);
	return ent;
}

#endif
