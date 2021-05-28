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
	Texture2D* normalMap;
	Texture2D* diffuseMap;
	Texture2D* specularMap;
	Texture2D* MetallicMap;
	Texture2D* RoughnessMap;
	Texture2D* OclussionMap;

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
