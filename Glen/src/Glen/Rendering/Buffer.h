#pragma once

#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <GL/glew.h>

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
	void setData(GLintptr offset, GLsizeiptr size, void* data, bool bind = false);
	void* mapToMemory(GLenum access, bool bind = false);
	void unmapFromMemroy(bool bind = false);
};
#endif // !UNIFORM_BUFFER_H
