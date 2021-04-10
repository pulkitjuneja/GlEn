//
// Created by Pulkit Juneja on 02/08/18.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"
#include "Glen/Globals.h"

class GLN_API Camera {
public:

	Transform transform;
	glm::vec3 up;
	glm::vec3 cameraUP;
	glm::vec3 right;
	glm::vec3 front;

	float nearPlane;
	float farPlane;
	float fov;
	float aspect;

	Camera(glm::vec3 position, glm::vec3 front, float fov, float aspect, float near, float far);
	void getFrontFromRotation();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::vec3* getFrustumCorners();
};

#endif