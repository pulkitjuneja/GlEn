#include "TAA.h"
#include "Glen/Core/EngineContext.h"
#include "Glen/ResourceManagement/ResourceManager.h"
#include "Glen/Utils/HaltonSequence.h"
#include "Glen/Scene/Scene.h"

void TAA::initializeTAAFbo(int fboId)
{
	aaFbos[fboId].bind();

	aaRenderTextures[fboId] = EngineContext::get()->resourceManager->generateTexture("TAA_color_" + std::to_string(fboId), TextureType::RENDERTEXTURE,
		SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_RGBA16F, GL_FLOAT);
	aaRenderTextures[fboId]->bind();
	aaRenderTextures[fboId]->setMinMagFilter(GL_LINEAR, GL_LINEAR);
	aaRenderTextures[fboId]->setWrapping(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	aaFbos[fboId].attachRenderTarget(aaRenderTextures[fboId], 0, 0);

	aaFbos[fboId].checkStatus();
	aaFbos[fboId].unBind();
}

void TAA::createJitterArray()
{
	for (int i = 0; i < 16; i++) {
		jitterArray[i] = glm::vec2(CreateHaltonSequence(i + 1, 2), CreateHaltonSequence(i + 1, 3));
	}
}

Texture2D* TAA::getActiveFBOTexture()
{
	return aaRenderTextures[activeFBO];
}

Texture2D* TAA::getOtherFBOTexture()
{
	return aaRenderTextures[1 - activeFBO];
}

void TAA::Initialize()
{
	TAAPass = EngineContext::get()->resourceManager->getShader("TAA");
	initializeTAAFbo(0);
	initializeTAAFbo(1);
	createJitterArray();
	glGenVertexArrays(1, &screenQuadVAO);
}

void TAA::updateUniforms(Buffer& TAAUniformBUffer)
{
	auto scene = EngineContext::get()->sceneManager;
	glm::vec2 pixelSize(1 / float(SCREEN_WIDTH), 1 / float(SCREEN_HEIGHT));
	glm::vec2 jitterOffset = jitterArray[jitterIndex] * pixelSize;
	glm::mat4 jitteredProjectionMat = scene->getMainCamera()->getProjectionMatrix();
	if (taaOn) {
		jitteredProjectionMat[2][0] = jitterOffset.x;
		jitteredProjectionMat[2][1] = jitterOffset.y;
		//if (EngineContext::get()->stats.frameCount % 200 == 0) {
		jitterIndex = (jitterIndex + 1) % 16;
		//}
	}

	taaUniforms.VPPrevNoJitter = scene->VPPrevNoJitter;
	taaUniforms.VPPrevJittered = scene->VPPrevJittered;
	taaUniforms.VPCurrentJittered = jitteredProjectionMat * scene->getMainCamera()->getViewMatrix();
	taaUniforms.VPCurrentJitteredInverse = glm::inverse(taaUniforms.VPCurrentJittered);
	taaUniforms.jitter = jitterOffset;
	taaUniforms.feedback = 0.75;
	scene->VPPrevNoJitter = scene->getMainCamera()->getProjectionMatrix() * scene->getMainCamera()->getViewMatrix();
	scene->VPPrevJittered = jitteredProjectionMat * scene->getMainCamera()->getViewMatrix();
	TAAUniformBUffer.setData(&taaUniforms, sizeof(taaUniforms), true);
}

void TAA::runTAAPass(FrameBuffer& HDRBuffer)
{
	activeFBO = flip ? 0 : 1;
	// ------------------- THIS IF CONDITION IS THE ASSHOLE HERE ------------------
	int frameCount = EngineContext::get()->stats.frameCount;
	if (EngineContext::get()->stats.frameCount == 1) {
		HDRBuffer.bind(GL_READ_FRAMEBUFFER);
		aaFbos[activeFBO].bind(GL_DRAW_FRAMEBUFFER);

		glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
			GL_COLOR_BUFFER_BIT, GL_LINEAR);
		aaFbos[activeFBO].unBind();
	}
	else {
		int otherFBO = 1 - activeFBO;
		aaRenderTextures[otherFBO]->bind(GL_TEXTURE0 + 19);
		TAAPass->setInt("currentColorTexture", 15);
		TAAPass->setInt("currentDepthTexture", 13);
		TAAPass->setInt("velocityTexture", 17);
		TAAPass->setInt("colorAntiAliased", 19);

		aaFbos[activeFBO].bind();
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);

		TAAPass->use();
		glBindVertexArray(screenQuadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		aaFbos[activeFBO].unBind();
	}


	flip = !flip;
}

