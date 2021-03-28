#include "Entity.h"

Entity::Entity(string name)
{
	this->name = name;
	this->DebugMesh = nullptr;
	this->mesh = nullptr;
}

Entity::Entity(string name, Mesh* mesh, Material* mat)
{
	this->name = name;
	this->mesh = mesh;
	this->DebugMesh = nullptr;
	this->overrideMaterial = mat;
}

Transform* Entity::getTransform()
{
	return &this->transfrom;
}

void Entity::attachDebugMesh(DebugMeshTypes debugDrawType)
{
	switch (debugDrawType) {
		case CUBE: this->DebugMesh = DebugDraw::createCubeMesh(); break;
		case CYLINDER: this->DebugMesh = DebugDraw::createCylinderMesh(); break;
		case SPHERE: this->DebugMesh = DebugDraw::createSphereMesh();break;
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

