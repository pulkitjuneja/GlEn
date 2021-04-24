#pragma once

#ifndef COLLIDER_H

#include "Glen/Globals.h"
#include "PhysicsContext.h"
#include "Glen/Core/EngineContext.h"

enum ColliderType
{
	Box,
	Sphere
};

struct Collider {
	PxMaterial* material;
	PxGeometry* colliderGeometry;
	glm::vec3 positionOffset;

protected:
	Collider(glm::vec3 positionOffset);
	virtual ~Collider() = default;
};

struct BoxCollider : public Collider {
	glm::vec3 halfExtents;
	BoxCollider(glm::vec3 halfExtents, glm::vec3 positionOffset = glm::vec3(0, 0, 0));
};

struct SphereCollider : public Collider {
	float radius;
	SphereCollider(float radius, glm::vec3 positionOffset = glm::vec3(0, 0, 0));
};


#endif // !COLLIDER_H
