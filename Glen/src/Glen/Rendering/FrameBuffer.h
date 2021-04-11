#pragma once

#include <GL/glew.h>

#include "Vertex.h"
#include "Texture.h"
#include "Glen/Core/Logger.h"

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class FrameBuffer {
	unsigned int fboId;
public:
	FrameBuffer();
	void attachDepthTarget(Texture* texture, uint32_t mipLevel, int layer = 0, bool shouldBind = false);
	void attachRenderTarget(Texture* texture, uint32_t mipLevel, int attachmentIndex, int layer = 0, bool shouldBind = false);
	void attachRenderBuffer(GLenum internalFormat, GLenum attachment, const uint32_t& w, const uint32_t& h, bool shouldBind = false);
	void checkStatus(bool shouldBind = false);
	void bind(int bindTarget = GL_FRAMEBUFFER);
	void unBind();
};

#endif