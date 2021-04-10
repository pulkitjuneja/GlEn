#pragma once 

#include "Glen/Globals.h"

#ifndef LIGHT_H
#define LIGHT_H 


struct Light {};

struct GLN_API DirectionalLight : public Light {
public:
	ALIGNED(16) glm::vec4 direction;
	ALIGNED(16) glm::vec4 diffuse;
	ALIGNED(16) glm::vec4 specular;
	ALIGNED(16) glm::vec4 ambient;
};

struct GLN_API PointLight : public Light {
public:
	ALIGNED(16) glm::vec4 position;
	ALIGNED(16) glm::vec4 diffuse;
	ALIGNED(16) glm::vec4 specular;
	ALIGNED(16) glm::vec4 ambient;
	ALIGNED(16) float radius;
	PointLight() {
		float constant = 1.0;
		float linear = 0.0025f;
		float quadratic = 0.00007f;
		float lightMax = std::fmaxf(std::fmaxf(diffuse.r, diffuse.g), diffuse.b);
		radius = 100;
	}
};

#endif
