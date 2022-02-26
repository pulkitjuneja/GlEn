#pragma once

#ifndef TAA_PASS
#define TAA_PASS

#include "Buffer.h"
#include "FrameBuffer.h"
#include <array>
#include "Uniforms.h"

class TAA {

	int jitterIndex = 0;
	std::array<glm::vec2, 16> jitterArray;
	TAAUniforms taaUniforms;
	Shader* TAAPass;
	std::array<Texture2D*, 2> aaRenderTextures;
	std::array<FrameBuffer, 2> aaFbos;
	// Empty FBO bound to draw full screen quad
	GLuint screenQuadVAO;
	bool flip = true;
	int activeFBO;

	void initializeTAAFbo(int fboID);
	void createJitterArray();
public:
	bool taaOn;

	Texture2D* getActiveFBOTexture();
	Texture2D* getOtherFBOTexture();
	void Initialize();
	void updateUniforms(Buffer& TAAUniformBUffer);
	void runTAAPass(FrameBuffer& HDRBuffer);
};

#endif // !TAA_PASS
