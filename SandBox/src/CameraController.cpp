#include "CameraController.h"

CameraController::CameraController(Camera* camera)
{
	this->camera = camera;
}

void CameraController::Update(float deltaTime)
{
	InputStatus* input = EngineContext::get()->inputStatus;
	handleRotation();
	if (input->isKeyPressed(Keys::W)) {
		camera->transform.translate(camera->front * CAMERA_SPEED);
	}
	if (input->isKeyPressed(Keys::S)) {
		camera->transform.translate(-camera->front * CAMERA_SPEED);
	}
	if (input->isKeyPressed(Keys::C)) {
		auto position = this->camera->transform.getPosition();
		auto rotation = this->camera->transform.getEulerAngles();
		std::cout << position.x << "  " << position.y << "  " << position.z << "\n";
		std::cout << rotation.x << "  " << rotation.y << "  " << rotation.z << "\n";
	}
}

void CameraController::handleRotation()
{
	InputStatus* input = EngineContext::get()->inputStatus;
	if (input->isMouseButtonPressed(MouseButtons::Left)) {
		glm::vec2 currentMousePosition = input->getMousePosition();
		if (firstMouseMove) {
			lastFrameMousePosition = currentMousePosition;
			firstMouseMove = false;
		}
		glm::vec2 mouseDelta = currentMousePosition - lastFrameMousePosition;
		glm::vec3 eulerAngleDelta(0);
		eulerAngleDelta.x = -mouseDelta.y;
		eulerAngleDelta.y = mouseDelta.x;
		eulerAngleDelta *= 0.05;
		camera->transform.rotate(eulerAngleDelta);
		lastFrameMousePosition = currentMousePosition;
	}
	else {
		firstMouseMove = true;
	}
}