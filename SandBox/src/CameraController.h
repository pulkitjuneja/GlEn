#pragma once 


#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "Glen.h"

class CameraController : public Script {

	Camera* camera;
	const float CAMERA_SPEED = 1.0f;
	glm::vec2 lastFrameMousePosition;
	void handleRotation();
	bool firstMouseMove = true;
public:
	void GLN_API Update(float deltaTimer) override;
	void GLN_API Start() override;

};

#endif // !CAMERA_CONTROLLER_H
