#pragma once

#include "Transform.h"

glm::mat4 Transform::getTransformationMatrix() {
	glm::quat rotationQuat = glm::quat(eulerAngles);
	glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);
	glm::mat4 transformationMAtrix(1.0f);
	transformationMAtrix = glm::scale(glm::mat4(1.0f), scale) * transformationMAtrix;
	transformationMAtrix = rotationMatrix * transformationMAtrix;
	transformationMAtrix = glm::translate(glm::mat4(1.0f), position) * transformationMAtrix;
	return transformationMAtrix;
}

void Transform::translate(glm::vec3  movement)
{
	position += movement;
}

void Transform::setPosition(glm::vec3 newPosition) {
	position = newPosition;
}

void Transform::increaseScale(glm::vec3  scale)
{
	this->scale += scale;
}

void Transform::rotate(glm::vec3  rotation)
{
	this->eulerAngles += rotation;
}

void Transform::setRotation(glm::vec3 rotation)
{
	this->eulerAngles = rotation;
}

glm::vec3 Transform::getPosition()
{
	return position;
}

glm::vec3 Transform::getScale()
{
	return scale;
}

glm::vec3 Transform::getEulerAngles()
{
	return eulerAngles;
}

glm::vec3 Transform::getForwardVector()
{
	glm::mat4 transformationMat = getTransformationMatrix();
	return glm::vec3(transformationMat[0][2], transformationMat[1][2], transformationMat[2][2]);
}

glm::vec3 Transform::getUpVector()
{
	glm::mat4 transformationMat = getTransformationMatrix();
	return glm::vec3(transformationMat[0][1], transformationMat[1][1], transformationMat[2][1]);
}

glm::vec3 Transform::getRightVector()
{
	glm::mat4 transformationMat = getTransformationMatrix();
	return glm::vec3(transformationMat[0][0], transformationMat[1][0], transformationMat[2][0]);
}

Transform::Transform()
{
	this->position = glm::vec3(0, 0, 0);
	this->eulerAngles = glm::vec3(0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);
}

void Transform::setScale(glm::vec3 scale)
{
	this->scale = scale;
}
