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

