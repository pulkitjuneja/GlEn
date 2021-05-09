#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "Glen/Globals.h"
#include "Transform.h"
#include "Glen/Rendering/Material.h"
#include "Script.h"
#include "Glen/Rendering/Mesh.h"
#include "Glen/Rendering/DebugDraw.h"
#include "Glen/Physics/Collider.h"
#include "Glen/Physics/RigidBody.h"
#include "Glen/Core/Logger.h"

enum DebugMeshTypes {
	CUBE,
	SPHERE,
	CYLINDER
};

class GLN_API Entity {

public:

	std::string name;
	Transform transfrom;
	Mesh* mesh;
	Material* overrideMaterial;
	Collider* collider;
	RigidBody rigidBody;

	// TODO: temporary remove
	glm::vec3 color;

	std::vector<Script*> scripts;

	Entity(std::string name);
	Entity(std::string name, Mesh* mesh, Material* mat);

	Transform* getTransform();

	void attachCollider(ColliderType colliderType);
	void attachBoxCollider(glm::vec3 halfExtents);
	void attachRigidBody(RigidBodyType type);
	void AddScript(Script* newScript);
};

#endif // !ENTITY_H
