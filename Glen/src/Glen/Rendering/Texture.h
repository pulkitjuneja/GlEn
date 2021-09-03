#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include "Glen/Core/Core.h"
#include "Glen/Globals.h"
#include "Shader.h"

enum class TextureType {
	DIFFUSE,
	SPECULAR,
	NORMAL,
	DEPTH,
	RENDERTEXTURE,
	METALLIC,
	ROUGHNESS,
	OCLUSSION,
	CUBEMAP
};

class Texture {
public:
	GLuint textureId;
	TextureType type;
	GLenum target;
	uint32_t w;
	uint32_t h;

	Texture(TextureType type, uint32_t w, uint32_t h);
	void bind(int textureUnit = GL_TEXTURE0);
	void Unbind();
	void generateMipMaps(bool bind = false);
	void setMinMagFilter(GLenum minFilter, GLenum magFilter, bool bind = false);
	virtual void setWrapping(GLenum s, GLenum t, GLenum r, bool bind = false) = 0;
};

class GLN_API Texture2D : public Texture {
public:

	GLenum format;
	GLenum internalFormat;
	GLenum dataType;

	Texture2D(TextureType type, uint32_t w, uint32_t h, GLenum format, GLenum dataType, GLenum internalFormat);
	void setData(void* data, bool bind = false);
	void setWrapping(GLenum s, GLenum t, GLenum r, bool bind = false);

};

class GLN_API Texture3D : public Texture {
public:

	int arraySize;
	GLenum format;
	GLenum internalFormat;
	GLenum dataType;

	Texture3D(TextureType type, int arraySize, uint32_t w, uint32_t h, GLenum format, GLenum dataType, GLenum internalFormat);
	void setData(void* data, int arrayIndex, bool bind = false);
	void setWrapping(GLenum s, GLenum t, GLenum r, bool bind = false);

};

// TODO: Merge this with the main texture class
class GLN_API CubeMap : public Texture {
public: 

	CubeMap(uint32_t w, uint32_t h);
	void setFaceData(int faceIndex, void* data, GLenum format,
	GLenum internalFormat, GLenum dataType, bool bind = false);
	void setWrapping(GLenum s, GLenum t, GLenum r, bool bind = false);

	void setDataFromHDRIMap(Texture2D* hdriMap);
	CubeMap* createConvolutionMap(const std::string& identifier);
	CubeMap* createPrefilteredEnvMap(const std::string& identifier);
	void renderCubeToFrameBuffer(Shader* shader, uint32_t w, uint32_t h, int mipLevels = 1);
};

#endif