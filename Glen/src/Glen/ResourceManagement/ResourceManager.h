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
#include "Glen/Rendering/Shader.h"
#include "Glen/Rendering/Mesh.h"
#include "Glen/Globals.h"
#include "Glen/Rendering/Vertex.h"
#include "Glen/Rendering/Texture.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

struct EnumClassHash
{
	template <typename T>
	std::size_t operator()(T t) const
	{
		return static_cast<std::size_t>(t);
	}
};

class GLN_API ResourceManager {
private:

	// Resource Maps
	std::unordered_map <std::string, Shader*> loadedShaders;
	std::unordered_map <string, Texture*> textures;
\

std::unordered_map<string, Mesh*> loadedMeshes;

	std::unordered_map <aiTextureType, TextureType, EnumClassHash> textureTypeMap = {
		{aiTextureType_DIFFUSE, TextureType::DIFFUSE},
		{aiTextureType_SPECULAR, TextureType::SPECULAR},
		{aiTextureType_HEIGHT, TextureType::NORMAL}
	};

	ResourceManager();
	static ResourceManager* instance;
	void readFromFile(const string& fileName, char*& shaderContent);
	Texture* loadMaterialTexture(aiMaterial* aiMaterial, aiTextureType textureType, string directory);
	Material* getAiSceneMaterial(const aiScene* scene, int materialIndex, string directory);

public:
	static ResourceManager* getInstance();

	//Resource Loaders
	void loadShader(const string& vertexShaderPath, const string& fragmentShaderPath, const string& shaderName);
	Texture* loadTexture(const string& texturePath, const string& directory, TextureType textureType);
	Mesh* loadMesh(string path, int loaderFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	Texture* generateTexture(const string& identifier, TextureType textureType, const uint32_t& w,
		const uint32_t& h, GLenum format, GLenum internalFormat, GLenum dataType, int arraySize);

	//Getters
	Shader* getShader(const string& shaderName);
	Texture* getTexture(const string& textureName);
};


#endif /* Resources_hpp */
