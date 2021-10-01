#pragma once 

#ifndef UNIFORMS_H
#define UNIFORMS_H 

#include <glm/glm.hpp>
#include "Light.h"


#define MAX_POINT_LIGHTS 10

struct PerFrameUniforms {
	ALIGNED(16) glm::mat4 projectionMatrix;
	ALIGNED(16) glm::mat4 viewMatrix;
	ALIGNED(16) glm::mat4 inverseProjectionMatrix;
	ALIGNED(16) glm::mat4 inverseViewMatrix;
	ALIGNED(16) glm::mat4 lightSpaceMatrix;
	ALIGNED(16) DirectionalLight directionalLight;
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

struct TAAUniforms {
	ALIGNED(16) glm::mat4 VPPrevNoJitter;
	ALIGNED(16) glm::mat4 VPPrevJittered;
	ALIGNED(16) glm::mat4 VPCurrentJittered;
	ALIGNED(16) glm::mat4 VPCurrentJitteredInverse;
	ALIGNED(16) glm::vec2 jitter;
	ALIGNED(16) float feedback;
};

#endif
