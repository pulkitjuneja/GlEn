#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer()
{
	csm = new Csm(0.3, 350.0f, 4, 4096);
	perFrameUbo = new Buffer(sizeof(PerFrameUniforms), 0, BufferType::UBO);
	CsmUbo = new Buffer(sizeof(CSMUniforms), 1, BufferType::UBO);
}

void ForwardRenderer::setupHDRBuffer()
{
	HDRBBuffer.bind();

	HDRBUfferTexture = EngineContext::get()->resourceManager->generateTexture(HDR_BUFFER_TEXTURE_NAME, TextureType::RENDERTEXTURE,
		SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_RGBA16F, GL_FLOAT);
	HDRBUfferTexture->bind();
	HDRBUfferTexture->setMinMagFilter(GL_NEAREST, GL_NEAREST);

	depthTexture = EngineContext::get()->resourceManager->generateTexture(DEPTH_TEXTURE_NAME, TextureType::DEPTH, SCREEN_WIDTH, SCREEN_HEIGHT,
		GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	depthTexture->bind();
	depthTexture->setMinMagFilter(GL_LINEAR, GL_LINEAR);
	depthTexture->setWrapping(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	HDRBBuffer.attachRenderTarget(HDRBUfferTexture, 0, 0);
	HDRBBuffer.attachDepthTarget(depthTexture, 0);

	HDRBBuffer.checkStatus();
	HDRBBuffer.unBind();
}

void ForwardRenderer::startup()
{
	basicToneMappingShader = EngineContext::get()->resourceManager->getShader("basicToneMapping");
	scene = EngineContext::get()->sceneManager;
	setupHDRBuffer();
	glGenVertexArrays(1, &screenQuadVAO);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void ForwardRenderer::shutdown() {}

void ForwardRenderer::update(float deltaTimer)
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	csm->update(scene->getMainCamera(), scene->getDirectionalLight().direction);
	csm->updateUniforms(csmUniforms);

	//Update Global Uniform blocks
	sceneRenderer.setGlobalUniforms(perFrameUniforms, scene);

	perFrameUbo->bind();
	void* mem = perFrameUbo->mapToMemory(GL_WRITE_ONLY);

	if (mem) {
		memcpy(mem, &perFrameUniforms, sizeof(PerFrameUniforms));
		perFrameUbo->unmapFromMemroy();
	}

	CsmUbo->bind();
	auto siz = sizeof(int);
	void* mem2 = CsmUbo->mapToMemory(GL_WRITE_ONLY);
	if (mem2) {
		memcpy(mem2, &csmUniforms, sizeof(CSMUniforms));
		CsmUbo->unmapFromMemroy();
	}

	csm->render(scene);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, HDRBBuffer.fboId);
	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneRenderer.renderScene(scene);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	toneMappingPass();

	HDRBBuffer.bind(GL_READ_FRAMEBUFFER);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void ForwardRenderer::toneMappingPass()
{
	basicToneMappingShader->setInt("hdrBuffer", 11);
	basicToneMappingShader->setFloat("exposure", 0.9f);
	HDRBUfferTexture->bind(GL_TEXTURE0 + 11);
	basicToneMappingShader->use();
	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}