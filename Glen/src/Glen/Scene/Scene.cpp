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

void Scene::update(float deltaTime)
{
	std::vector<Entity*>::iterator it = Entities.begin();
	for (; it != Entities.end(); it++) {

		(*it)->update(deltaTime);


		if ((*it)->rigidBody != nullptr) {

			Entity* ent = (*it);
			PxRigidActor* rb = ent->rigidBody->getNativeRigidBody();
			PxShape* shapes[1];
			rb->getShapes(shapes, 1);
			PxTransform world = PxShapeExt::getGlobalPose(*shapes[0], *rb);

			glm::vec3 position(world.p.x, world.p.y, world.p.z);

			PxQuat q = world.q;

			double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
			double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
			double roll = std::atan2(sinr_cosp, cosr_cosp);

			double pitch;
			double sinp = 2 * (q.w * q.y - q.z * q.x);
			if (std::abs(sinp) >= 1)
				pitch = std::copysign(3.1415 / 2, sinp); // use 90 degrees if out of range
			else
				pitch = std::asin(sinp);

			double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
			double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
			double yaw = std::atan2(siny_cosp, cosy_cosp);

			// Hacky fix to attach rigidbody at the center of the mesh
			ent->transfrom.setPosition(position);
			ent->transfrom.setRotation(glm::vec3(roll, pitch, yaw));
		}
	}

	// update entity transforms based on rigidBody
}
