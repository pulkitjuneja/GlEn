#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Glen/Core/Core.h"
#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"

class GLN_API Material {

public:
	std::string name;
	Shader* getShader() { return shader; };
	void setShader(Shader* shader) { this->shader = shader; };
	Texture* normalMap;
	Texture* diffuseMap;
	Texture* specularMap;
	Texture* MetallicMap;
	Texture* RoughnessMap;
	Texture* OclussionMap;

	Material() {
		normalMap = nullptr;
		diffuseMap = nullptr;
		specularMap = nullptr;
		MetallicMap = nullptr;
		RoughnessMap = nullptr;
		OclussionMap = nullptr;
	}
private:
	Shader* shader;
};


#endif //MATERIAL_H
