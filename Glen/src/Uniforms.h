#pragma once 

#include "Globals.h"
#include <glm/glm.hpp>
#include "Light.h"

#ifndef UNIFORMS_H
#define UNIFORMS_H 

#define MAX_POINT_LIGHTS 10

struct PerFrameUniforms {
	ALIGNED(16) glm::mat4 projectionMatrix;
	ALIGNED(16) glm::mat4 viewMatrix;
	ALIGNED(16) DirectionalLight directionalLight;
	ALIGNED(16) PointLight pointLights[MAX_POINT_LIGHTS];
	ALIGNED(16) glm::vec4 cameraPosition;
	int pointLightCount;
};

#endif
