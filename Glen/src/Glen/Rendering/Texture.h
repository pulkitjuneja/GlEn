#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include "Glen/Core/Core.h"
#include "Glen/Globals.h"

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

class GLN_API Texture {
public:

	GLuint textureId;
	TextureType type;
	GLenum target;
	int arraySize;
	GLenum format;
	GLenum internalFormat;
	uint32_t w;
	uint32_t h;
	GLenum dataType;

	Texture(TextureType type, int arraySize, uint32_t w, uint32_t h, GLenum format, GLenum dataType, GLenum internalFormat);
	void bind(int textureUnit = GL_TEXTURE0);
	void Unbind();
	void setData(void* data, int arrayIndex, bool bind = false);
	void generateMipMaps(bool bind = false);
	void setMinMagFilter(GLenum minFilter, GLenum magFilter, bool bind = false);
	void setWrapping(GLenum s, GLenum t, bool bind = false);

};

// TODO: Merge this with the main texture class
class GLN_API CubeMap {
public: 
	GLuint textureId;
	uint32_t w;
	uint32_t h;

	CubeMap(uint32_t w, uint32_t h);
	void setFaceData(int faceIndex, void* data, GLenum format,
	GLenum internalFormat, GLenum dataType, bool bind = false);
	void bind(int textureUnit = GL_TEXTURE0);
	void unBind();
	void setMinMagFilter(GLenum minFilter, GLenum magFilter, bool bind = false);
	void setWrapping(GLenum s, GLenum t, GLenum r, bool bind = false);
};

#endif