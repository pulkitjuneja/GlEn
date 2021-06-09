#include "UniformBuffer.h"

Buffer::Buffer(GLsizeiptr bufferSize, GLuint bindIndex, BufferType bufferType)
{
	glGenBuffers(1, &id);
	target = bufferType;
	glBindBuffer(target, id);
	glBufferData(target, bufferSize, nullptr, GL_STATIC_DRAW);
	glBindBuffer(target, 0);

	glBindBufferBase(target, bindIndex, id);
}

void Buffer::bind()
{
	glBindBuffer(target, id);
}

void Buffer::unBind()
{
	glBindBuffer(target, 0);
}

void Buffer::setData(GLintptr offset, GLsizeiptr size, void* data, bool bind)
{
	if (bind) {
		glBindBuffer(target, id);
	}
	glBufferSubData(target, offset, size, data);
	if (bind) {
		glBindBuffer(target, 0);
	}
}

void* Buffer::mapToMemory(GLenum access, bool bind) {
	if (bind) {
		glBindBuffer(target, id);
	}
	void* ptr = glMapBuffer(target, access);
	if (bind) {
		glBindBuffer(target, 0);
	}
	return ptr;
}

void Buffer::unmapFromMemroy(bool bind)
{
	if (bind) {
		glBindBuffer(target, id);
	}
	glUnmapBuffer(target);
	if (bind) {
		glBindBuffer(target, 0);
	}
}
