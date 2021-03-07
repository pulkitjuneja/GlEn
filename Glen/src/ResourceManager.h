//
//  Resources.hpp
//  src
//
//  Created by Pulkit Juneja on 22/07/18.
//  Copyright © 2018 Pulkit Juneja. All rights reserved.
//

#ifndef Resources_hpp
#define Resources_hpp

#include <stdio.h>
#include <unordered_map> 
#include "Shader.h"
#include "Mesh.h"
#include "Globals.h"
#include "Vertex.h"
#include "Texture.h"

struct EnumClassHash
{
	template <typename T>
	std::size_t operator()(T t) const
	{
		return static_cast<std::size_t>(t);
	}
};

class ResourceManager {
private:

	// Resource Maps
	std::unordered_map <string, Shader*> loadedShaders;

	ResourceManager();
	static ResourceManager* instance;
	void readFromFile(const string& fileName, char*& shaderContent);

public:
	static ResourceManager* getInstance();

	//Resource Loaders
	void loadShader(const string& vertexShaderPath, const string& fragmentShaderPath, const string& shaderName);

	//Getters
	Shader* getShader(const string& shaderName);
};


#endif /* Resources_hpp */
