#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "Glen/Globals.h"
#include "Transform.h"
#include "Glen/Rendering/Material.h"
#include "Script.h"
#include "Glen/Rendering/Mesh.h"
#include "Glen/Rendering/DebugDraw.h"


enum DebugMeshTypes {
	CUBE,
	SPHERE,
	CYLINDER
};


class GLN_API Entity {

public:

	string name;
	Transform transfrom;
	Mesh* mesh;
	Mesh* DebugMesh;
	Material* overrideMaterial;
	std::vector<Script*> scripts;

	Entity(string name);
	Entity(string name, Mesh* mesh, Material* mat);

	Transform* getTransform();

	void attachDebugMesh(DebugMeshTypes debugMeshType);
	void AddScript(Script* newScript);
	void update(float deltaTime);
};

#endif // !ENTITY_H
