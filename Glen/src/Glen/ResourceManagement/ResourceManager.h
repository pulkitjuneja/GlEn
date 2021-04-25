//
//  Resources.hpp
//  src
//
//  Created by Pulkit Juneja on 22/07/18.
//  Copyright � 2018 Pulkit Juneja. All rights reserved.
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
#include "Glen/Core/Logger.h"

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
	std::unordered_map <std::string, Texture*> textures;
	std::unordered_map <std::string, Mesh*> loadedMeshes;
	std::unordered_map <aiTextureType, TextureType, EnumClassHash> textureTypeMap = {
		{aiTextureType_DIFFUSE, TextureType::DIFFUSE},
		{aiTextureType_SPECULAR, TextureType::SPECULAR},
		{aiTextureType_HEIGHT, TextureType::NORMAL}
	};

	void readFromFile(const std::string& fileName, char*& shaderContent);
	Texture* loadMaterialTexture(aiMaterial* aiMaterial, aiTextureType textureType, std::string directory);
	Material* getAiSceneMaterial(const aiScene* scene, int materialIndex, std::string directory);

public:

	//Resource Loaders
	void loadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& shaderName);
	Texture* loadTexture(const std::string& texturePath, const std::string& directory, TextureType textureType);
	Mesh* loadMesh(std::string path, int loaderFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	Texture* generateTexture(const std::string& identifier, TextureType textureType, const uint32_t& w,
		const uint32_t& h, GLenum format, GLenum internalFormat, GLenum dataType, int arraySize);

	//Getters
	Shader* getShader(const std::string& shaderName);
	Texture* getTexture(const std::string& textureName);

	ResourceManager() {};
};


#endif /* Resources_hpp */
