#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "Globals.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Script.h"

class GLN_API Entity {

public:

	string name;
	Transform transfrom;
	Mesh* mesh;
	Material* overrideMaterial;
	std::vector<Script*> scripts;

	Entity(string name);
	Entity(string name, Mesh* mesh, Material* mat);

	Transform* getTransform();

	void AddScript(Script* newScript);
	void update(float deltaTime);
};

#endif // !ENTITY_H
