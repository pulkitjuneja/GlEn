#include "Entity.h"

Entity::Entity(string name)
{
	this->name = name;
	this->collider = nullptr;
	this->mesh = nullptr;
	this->rigidBody = nullptr;
}

Entity::Entity(string name, Mesh* mesh, Material* mat)
{
	this->name = name;
	this->mesh = mesh;
	this->collider = nullptr;
	this->overrideMaterial = mat;
	this->rigidBody = nullptr;
}

Transform* Entity::getTransform()
{
	return &this->transfrom;
}

void Entity::attachCollider(ColliderType colliderType)
{
	if (colliderType == ColliderType::Box) {
		auto bounds = mesh->bounds.getExtents();
		collider = new BoxCollider(glm::vec3(bounds.x/2, bounds.y/2, bounds.z/2));
	}
	else if (colliderType == ColliderType::Sphere) {
		auto bounds = mesh->bounds.getExtents();
		float radius = std::max(bounds.x, std::max(bounds.y, bounds.z)) / 2;
		collider = new SphereCollider(radius);
	}
	else {
		Logger::logWarn("INvalid colider type specified to " + name);
	}
}

void Entity::attachRigidBody()
{
	if (collider == nullptr) {
		Logger::logWarn("No collider attached cannot attach rigidBody");
		return;
	}
	else {
		rigidBody = new RigidBody(transfrom, collider);
	}
}

void Entity::AddScript(Script* newScript)
{
	newScript->attachParentEntity(this);
	newScript->Start();
	scripts.push_back(newScript);
}

void Entity::update(float deltaTime) {
	for (auto script : scripts) {
		script->Update(deltaTime);
	}
}

