#pragma once

#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <GL/glew.h>

class UniformBuffer {
	GLuint id;
public:
	UniformBuffer(GLsizeiptr bufferSize, GLuint bindIndex);
	void bind();
	void unBind();
	void setData(GLintptr offset, GLsizeiptr size, void* data, bool bind = false);
	void* mapToMemory(GLenum access, bool bind = false);
	void unmapFromMemroy(bool bind = false);
};

#endif // !UNIFORM_BUFFER_H
