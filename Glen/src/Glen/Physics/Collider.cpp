#include "Collider.h"

Collider::Collider(glm::vec3 positionOffset) : positionOffset(positionOffset),
material(EngineContext::get()->physicsContext->defaultMaterial){}

BoxCollider::BoxCollider(glm::vec3 halfExtents, glm::vec3 positionOffset) : halfExtents(halfExtents),
Collider(positionOffset) {
	colliderGeometry = new PxBoxGeometry(PxVec3(halfExtents.x, halfExtents.y, halfExtents.z));
}

SphereCollider::SphereCollider(float radius, glm::vec3 positionOffset): radius(radius),
Collider(positionOffset){
	colliderGeometry = new PxSphereGeometry(radius);
}
