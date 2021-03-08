#pragma once

#include "Scene.h"

PointLight* Scene::createPointLight(glm::vec4 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
{
	PointLight* light = new PointLight();
	light->position = position;
	light->ambient = ambient;
	light->diffuse = diffuse;
	light->specular = specular;

	pointLights.push_back(light);
	return light;
}

DirectionalLight* Scene::createDirectionalLight(glm::vec4 direction, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
{
	DirectionalLight* light = new DirectionalLight();

	light->ambient = ambient;
	light->diffuse = diffuse;
	light->specular = specular;
	light->direction = direction;

	this->directionalLight = light;
	return light;

}

std::vector<Entity*>& Scene::getEntities()
{
	return Entities;
}

std::vector<PointLight*>& Scene::getPointLIghts()
{
	return pointLights;
}

DirectionalLight* Scene::getDirectionalLight()
{
	return directionalLight;
}

Camera* Scene::getMainCamera()
{
	return mainCamera;
}

void  Scene::setMainCamera(Camera* camera)
{
	this->mainCamera = camera;
}

void Scene::update()
{
	std::vector<Entity*>::iterator it = Entities.begin();
	for (; it != Entities.end(); it++) {
		(*it)->update();
	}
}
