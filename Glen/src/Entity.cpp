#include "Entity.h"

Entity::Entity(string name)
{
	this->name = name;
}

Entity::Entity(string name, Mesh* mesh, Material* mat)
{
	this->name = name;
	this->mesh = mesh;
	this->overrideMaterial = mat;
}

Transform* Entity::getTransform()
{
	return &this->transfrom;
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

