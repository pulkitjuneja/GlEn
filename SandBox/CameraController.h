#pragma once 


#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <Camera.h>
#include <Globals.h>

class CameraController {

	Camera* camera;
	const float CAMERA_SPEED = 1.0f;
	glm::vec2 lastFrameMousePosition;
	void handleRotation();
	bool firstMouseMove = true;
public:
	CameraController(Camera* camera);
	void update();

};

#endif // !CAMERA_CONTROLLER_H
