#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 eulerAngles, float fov, float aspect, float near, float far) : front(front),
fov(fov),
nearPlane(near),
farPlane(far),
aspect(aspect) {
	fov = fov / 57.2957795 + 0.2f;
	this->transform.setPosition(position);
	this->transform.setRotation(eulerAngles);
	this->up = glm::vec3(0, 1, 0);
	getFrontFromRotation();
	this->cameraUP = glm::normalize(glm::cross(right, front));
}

void Camera::getFrontFromRotation()
{
	glm::vec3 eulerAngles = transform.getEulerAngles();
	if (eulerAngles.x > 89) {
		eulerAngles.x = 89;
	}
	else if (eulerAngles.x < -89) {
		eulerAngles.x = -89;
	}

	glm::vec3 newFront;
	newFront.x = cos(glm::radians(eulerAngles.y)) * cos(glm::radians(eulerAngles.x));
	newFront.y = sin(glm::radians(eulerAngles.x));
	newFront.z = cos(glm::radians(eulerAngles.x)) * sin(glm::radians(eulerAngles.y));
	front = glm::normalize(newFront);
	right = glm::normalize(glm::cross(front, up));
}

glm::mat4 Camera::getViewMatrix()
{
	getFrontFromRotation();
	return glm::lookAt(this->transform.getPosition(), this->transform.getPosition() + front, this->up);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(glm::radians(80.0f), aspect, nearPlane, farPlane);
}

glm::vec3* Camera::getFrustumCorners()
{
	glm::vec3 center = this->transform.getPosition();

	glm::vec3 fc = center + front * farPlane;
	glm::vec3 nc = center + front * nearPlane;

	float nearHeight = tan(fov / 2.0f) * nearPlane;
	float nearWidth = nearHeight * aspect;
	float farHeight = tan(fov / 2.0f) * farPlane;
	float farWidth = farHeight * aspect;

	glm::vec3* corners = new glm::vec3[8];

	corners[0] = nc - (cameraUP * nearHeight) - (right * nearWidth); // near-bottom-left
	corners[1] = nc + (cameraUP * nearHeight) - (right * nearWidth); // near-top-left
	corners[2] = nc + (cameraUP * nearHeight) + (right * nearWidth); // near-top-right
	corners[3] = nc - (cameraUP * nearHeight) + (right * nearWidth); // near-bottom-right

	corners[4] = fc - (cameraUP * farHeight) - (right * farWidth); // far-bottom-left
	corners[5] = fc + (cameraUP * farHeight) - (right * farWidth); // far-top-left
	corners[6] = fc + (cameraUP * farHeight) + (right * farWidth); // far-top-right
	corners[7] = fc - (cameraUP * farHeight) + (right * farWidth); // far-bottom-right

	return corners;
}