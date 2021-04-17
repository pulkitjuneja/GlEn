#include "Collider.h"

Collider::Collider(glm::vec3 positionOffset) : positionOffset(positionOffset),
	material(PhysicsContext::get()->defaultMaterial){}

BoxCollider::BoxCollider(glm::vec3 halfExtents, glm::vec3 positionOffset) : halfExtents(halfExtents),
	Collider(positionOffset) {}

SphereCollider::SphereCollider(float radius, glm::vec3 positionOffset): radius(radius), 
Collider(positionOffset){}
