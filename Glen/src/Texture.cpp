
#include "Texture.h"

// TODO: Add support for mip map levels 
Texture::Texture(TextureType type, int arraySize, uint32_t w, uint32_t h, GLenum format, GLenum dataType, GLenum internalFormat) {
	glGenTextures(1, &textureId);

	this->type = type;
	this->arraySize = arraySize;
	this->w = w;
	this->h = h;
	this->format = format;
	this->internalFormat = internalFormat;
	this->dataType = dataType;

	if (arraySize > 1) {
		target = GL_TEXTURE_2D_ARRAY;
		glBindTexture(target, textureId);
		glTexImage3D(target, 0, internalFormat, w, h, arraySize, 0, format, dataType, NULL);
	}
	else {
		target = GL_TEXTURE_2D;
		glBindTexture(target, textureId);
		glTexImage2D(target, 0, internalFormat, w, h, 0, format, dataType, NULL);
	}
	glBindTexture(target, 0);
}

//Texture::~Texture() {
//	std::cout << "Destructor called";
//	glDeleteTextures(1, &textureId);
//}

void Texture::bind(int textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(target, textureId);
}

void Texture::setData(void* data, int arrayIndex, bool bind) {
	if (bind) {
		glBindTexture(target, textureId);
	}
	if (arrayIndex > 0) {
		glTexImage3D(target, 0, internalFormat, w, h, arrayIndex, 0, format, dataType, data);
	}
	else {
		glTexImage2D(target, 0, internalFormat, w, h, 0, format, dataType, data);
	}
	if (bind) {
		Unbind();
	}
}

void Texture::generateMipMaps(bool bind)
{
	if (bind) {
		glBindTexture(target, textureId);
	}
	glGenerateMipmap(target);
	if (bind) {
		Unbind();
	}
}

void Texture::Unbind()
{
	glBindTexture(target, 0);
}

void Texture::setMinMagFilter(GLenum minFilter, GLenum magFilter, bool bind) {
	if (bind) {
		glBindTexture(target, textureId);
	}
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
	if (bind) {
		Unbind();
	}
}

void Texture::setWrapping(GLenum s, GLenum t, bool bind) {
	if (bind) {
		glBindTexture(target, textureId);
	}
	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	if (bind) {
		Unbind();
	}
}
