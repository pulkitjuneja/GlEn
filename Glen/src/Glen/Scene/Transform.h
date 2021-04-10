#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Glen/Globals.h"

class GLN_API Transform {
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 eulerAngles;
public:

	Transform();
	glm::mat4 getTransformationMatrix();
	void setPosition(glm::vec3 newPosition);
	void translate(glm::vec3 movement);
	void increaseScale(glm::vec3 scale);
	void setScale(glm::vec3 scale);
	void rotate(glm::vec3 rotation);
	void setRotation(glm::vec3 rotation);

	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec3 getEulerAngles();
	glm::vec3 getForwardVector();
	glm::vec3 getUpVector();
	glm::vec3 getRightVector();
};


#endif // !TRANSFORM_H
