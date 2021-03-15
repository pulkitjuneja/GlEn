#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "Globals.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

class GLN_API Entity {

public:

	string name;
	Transform transfrom;
	Mesh* mesh;
	Material* overrideMaterial;

	Entity(string name);
	Entity(string name, Mesh* mesh, Material* mat);
	Transform* getTransform();
	virtual void update() {};
};

#endif // !ENTITY_H
