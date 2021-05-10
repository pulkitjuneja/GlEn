#pragma once

#include "Scene.h"

PointLight& SceneManager::createPointLight(glm::vec4 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
{
	PointLight light;
	light.position = position;
	light.ambient = ambient;
	light.diffuse = diffuse;
	light.specular = specular;

	pointLights.push_back(light);
	return pointLights[pointLights.size()-1];
}

DirectionalLight& SceneManager::createDirectionalLight(glm::vec4 direction, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
{
	DirectionalLight light;

	light.ambient = ambient;
	light.diffuse = diffuse;
	light.specular = specular;
	light.direction = direction;

	this->directionalLight = light;
	return light;

}

std::vector<Entity>& SceneManager::getEntities()
{
	return Entities;
}

std::vector<PointLight>& SceneManager::getPointLIghts()
{
	return pointLights;
}

DirectionalLight& SceneManager::getDirectionalLight()
{
	return directionalLight;
}

Camera* SceneManager::getMainCamera()
{
	return mainCamera;
}

void SceneManager::Release()
{
	this->Entities.clear();
	this->pointLights.clear();
}

void  SceneManager::setMainCamera(glm::vec3 position, glm::vec3 front, float fov, float aspect, float nearPlane, float farPlane)
{
	this->mainCamera = Mem::Allocate<Camera>(position, front, fov, aspect, nearPlane, farPlane);
}