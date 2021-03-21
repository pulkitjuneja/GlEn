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
	ALIGNED(16) glm::mat4 lightSpaceMatrix;
	ALIGNED(16) DirectionalLight directionalLight;
	ALIGNED(16) PointLight pointLights[MAX_POINT_LIGHTS];
	ALIGNED(16) glm::vec4 cameraPosition;
	int pointLightCount;
};

struct FarBound {
	ALIGNED(16) float farBound;
};

struct CSMUniforms {
	//ALIGNED(16) glm::mat4 cropMatrices[MAX_FRUSTUM_SPLITS];
	int splitCount;
	ALIGNED(16) glm::mat4 textureMatrices[CSM_MAX_SPLIT_COUNT];
	ALIGNED(16) FarBound farBounds[CSM_MAX_SPLIT_COUNT];

};

#endif