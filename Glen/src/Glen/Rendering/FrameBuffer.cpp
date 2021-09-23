#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {
	glGenFramebuffers(1, &fboId);
}

void FrameBuffer::bind(int bindTarget) {
	glBindFramebuffer(bindTarget, fboId);
}

void FrameBuffer::unBind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::attachDepthTarget(Texture* texture, uint32_t mipLevel, int layer, bool shouldBind) {
	if (shouldBind) {
		texture->bind(GL_TEXTURE0);
		bind();
	}

	if (dynamic_cast<Texture3D*>(texture)) {
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->textureId, mipLevel, layer);
	}
	else {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->target, texture->textureId, mipLevel);
	}

	if (shouldBind) {
		unBind();
		texture->Unbind();
	}
}

void FrameBuffer::attachRenderTarget(Texture* texture, uint32_t mipLevel, int attachmentIndex, int layer, bool shouldBind)
{
	if (shouldBind) {
		texture->bind(GL_TEXTURE0);
		bind();
	}

	if (dynamic_cast<Texture3D*>(texture)) {
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex, texture->textureId, mipLevel, layer);
	}
	else {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex, texture->target, texture->textureId, mipLevel);
	}

	if (shouldBind) {
		unBind();
		texture->Unbind();
	}
}

int FrameBuffer::attachRenderBuffer(GLenum internalFormat, GLenum attachment, const uint32_t& w, const uint32_t& h, bool shouldBind)
{
	if (shouldBind) {
		bind();
	}
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo);
	return rbo;
	if (shouldBind) {
		unBind();
	}
}

void FrameBuffer::checkStatus(bool shouldBind)
{
	if (shouldBind) {
		bind();
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::string error = "Framebuffer Incomplete: ";

		switch (status)
		{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		{
			error += "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		{
			error += "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		{
			error += "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		{
			error += "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		}
		case GL_FRAMEBUFFER_UNSUPPORTED:
		{
			error += "GL_FRAMEBUFFER_UNSUPPORTED";
			break;
		}
		default:
			error += "NOT";
			break;
		}
		Logger::logError(error);
	}

	if (shouldBind) {
		unBind();
	}
}
