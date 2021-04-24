#pragma once

#include "Scene.h"

PointLight* SceneManager::createPointLight(glm::vec4 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
{
	PointLight* light = new PointLight();
	light->position = position;
	light->ambient = ambient;
	light->diffuse = diffuse;
	light->specular = specular;

	pointLights.push_back(light);
	return light;
}

DirectionalLight* SceneManager::createDirectionalLight(glm::vec4 direction, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
{
	DirectionalLight* light = new DirectionalLight();

	light->ambient = ambient;
	light->diffuse = diffuse;
	light->specular = specular;
	light->direction = direction;

	this->directionalLight = light;
	return light;

}

std::vector<Entity*>& SceneManager::getEntities()
{
	return Entities;
}

std::vector<PointLight*>& SceneManager::getPointLIghts()
{
	return pointLights;
}

DirectionalLight* SceneManager::getDirectionalLight()
{
	return directionalLight;
}

Camera* SceneManager::getMainCamera()
{
	return mainCamera;
}

void  SceneManager::setMainCamera(Camera* camera)
{
	this->mainCamera = camera;
}