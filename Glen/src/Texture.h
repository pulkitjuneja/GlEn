#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "Globals.h"

enum class TextureType {
	DIFFUSE,
	SPECULAR,
	NORMAL,
	DEPTH
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

#endif