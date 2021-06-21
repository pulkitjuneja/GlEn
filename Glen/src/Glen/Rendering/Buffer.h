#pragma once

#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <GL/glew.h>
#include "Glen/Globals.h"

enum BufferType {
	UBO = GL_UNIFORM_BUFFER,
	SSBO = GL_SHADER_STORAGE_BUFFER
};

class Buffer {
	GLuint id;
	GLuint target;
public:
	Buffer(GLsizeiptr bufferSize, GLuint bindIndex, BufferType bufferType);
	void bind();
	void unBind();
	void setData(void* data, size_t size, bool bind = false);
	void* mapForRead(bool bind = false);
	void unmap(bool bind = false);
};
#endif // !UNIFORM_BUFFER_H
