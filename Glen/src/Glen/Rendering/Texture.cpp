
#include "Texture.h"

// TODO: Add support for mip map levels 
Texture2D::Texture2D(TextureType type, uint32_t w, uint32_t h, GLenum format, GLenum dataType, GLenum internalFormat) : Texture (type, w, h) {

	this->format = format;
	this->internalFormat = internalFormat;
	this->dataType = dataType;

	target = GL_TEXTURE_2D;
	glBindTexture(target, textureId);
	glTexImage2D(target, 0, internalFormat, w, h, 0, format, dataType, NULL);
	glBindTexture(target, 0);
}

//Texture::~Texture() {
//	std::cout << "Destructor called";
//	glDeleteTextures(1, &textureId);
//}

void Texture2D::setData(void* data, bool bind) {
	if (bind) {
		glBindTexture(target, textureId);
	}

	glTexImage2D(target, 0, internalFormat, w, h, 0, format, dataType, data);

	if (bind) {
		Unbind();
	}
}



void Texture2D::setWrapping(GLenum s, GLenum t, GLenum r, bool bind) {
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

CubeMap::CubeMap(uint32_t w, uint32_t h) : Texture(TextureType::CUBEMAP, w, h){
	target = GL_TEXTURE_CUBE_MAP;
}

void CubeMap::setFaceData(int faceIndex, void* data, GLenum format,
	GLenum internalFormat, GLenum dataType, bool bind)
{
	if (bind) {
		glBindTexture(target, textureId);
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, 0, internalFormat, w, h, 0, format, dataType, data);

	if (bind) {
		Unbind();
	}
}

void CubeMap::setWrapping(GLenum s, GLenum t, GLenum r, bool bind)
{
	if (bind) {
		glBindTexture(target, textureId);
	}

	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, r);

	if (bind) {
		Unbind();
	}
}

Texture::Texture(TextureType type, uint32_t w, uint32_t h)
{
	glGenTextures(1, &textureId);

	this->type = type;
	this->w = w;
	this->h = h;
}

void Texture::bind(int textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(target, textureId);
}

void Texture::Unbind()
{
	glBindTexture(target, 0);
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

void Texture::setMinMagFilter(GLenum minFilter, GLenum magFilter, bool bind)
{
	if (bind) {
		glBindTexture(target, textureId);
	}

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);

	if (bind) {
		Unbind();
	}
}

Texture3D::Texture3D(TextureType type, int arraySize, uint32_t w, uint32_t h, GLenum format, GLenum dataType, GLenum internalFormat) : Texture(type, w, h)
{
	this->format = format;
	this->internalFormat = internalFormat;
	this->dataType = dataType;
	this->arraySize = arraySize;

	target = GL_TEXTURE_2D_ARRAY;
	glBindTexture(target, textureId);
	glTexImage3D(target, 0, internalFormat, w, h, arraySize, 0, format, dataType, NULL);
}

void Texture3D::setData(void* data, int arrayIndex, bool bind)
{
	if (bind) {
		glBindTexture(target, textureId);
	}

	glTexImage3D(target, 0, internalFormat, w, h, arrayIndex, 0, format, dataType, data);

	if (bind) {
		Unbind();
	}
}

void Texture3D::setWrapping(GLenum s, GLenum t, GLenum r, bool bind)
{
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
