#include "ForwardRenderer.h"
#include "Glen/Core/Input.h"

ForwardRenderer::ForwardRenderer()
{
	csm = Mem::Allocate<Csm>(0.3, 350.0f, 4, 4096);
	perFrameUbo = Mem::Allocate<Buffer>(sizeof(PerFrameUniforms), 0, BufferType::UBO);
	CsmUbo = Mem::Allocate<Buffer>(sizeof(CSMUniforms), 1, BufferType::UBO);
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
	depthTexture->setMinMagFilter(GL_NEAREST, GL_NEAREST);
	depthTexture->setWrapping(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);

	HDRBBuffer.attachRenderTarget(HDRBUfferTexture, 0, 0);
	HDRBBuffer.attachDepthTarget(depthTexture, 0);

	CsmUbo = Mem::Allocate<Buffer>(sizeof(CSMUniforms), 1, BufferType::UBO);

	HDRBBuffer.checkStatus();
	HDRBBuffer.unBind();
}

void ForwardRenderer::startup()
{
	basicToneMappingShader = EngineContext::get()->resourceManager->getShader("basicToneMapping");
	depthPassMaterial.setShader(EngineContext::get()->resourceManager->getShader("depthPass"));
	LightCullingCompute = dynamic_cast<ComputeShader*>(EngineContext::get()->resourceManager->getShader("LightCullingCompute"));
	LightCullingDebugShader = EngineContext::get()->resourceManager->getShader("lightDebugShader");

	scene = EngineContext::get()->sceneManager;
	setupHDRBuffer();
	glGenVertexArrays(1, &screenQuadVAO);

	int workGroupsX = (SCREEN_WIDTH + (SCREEN_WIDTH % 16)) / 16;
	int workGroupsY = (SCREEN_HEIGHT + (SCREEN_HEIGHT % 16)) / 16;
	size_t numberOfTiles = workGroupsX * workGroupsY;

	visibleLightBuffer = Mem::Allocate<Buffer>(numberOfTiles * sizeof(int) * MAX_LIGHTS_PER_TILE, 3, BufferType::SSBO);
	DebugDepthBuffer = Mem::Allocate<Buffer>(numberOfTiles * sizeof(float) , 4, BufferType::SSBO);
	pointLightBuffer = Mem::Allocate<Buffer>(sizeof(PointLight) * MAXPOINTLIGHTS, 5, BufferType::SSBO);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void ForwardRenderer::shutdown() {}

void ForwardRenderer::update(float deltaTimer)
{
	if (EngineContext::get()->inputStatus->isKeyPressed(Keys::C)) {
		debug = !debug;
	}

	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update shadow frustums
	csm->update(scene->getMainCamera(), scene->getDirectionalLight().direction);

	//Update Global Uniform blocks
	csm->updateUniforms(csmUniforms);
	sceneRenderer.setGlobalUniforms(perFrameUniforms, scene);
	pointLightBuffer->setData(&scene->getPointLIghts()[0], sizeof(PointLight) * scene->getPointLIghts().size(), true);

	perFrameUbo->setData(&perFrameUniforms, sizeof(perFrameUniforms), true);
	CsmUbo->setData(&csmUniforms, sizeof(csmUniforms), true);

	// render shadow maps
	csm->render(scene);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	HDRBBuffer.bind();
	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Depth Prepass;
	glDepthFunc(GL_LESS); 
	sceneRenderer.renderScene(scene, &depthPassMaterial, true);
	depthTexture->bind(GL_TEXTURE0 + 12);

	// light culling
	int workGroupsX = (SCREEN_WIDTH + (SCREEN_WIDTH % 16)) / 16;
	int workGroupsY = (SCREEN_HEIGHT + (SCREEN_HEIGHT % 16)) / 16;
	LightCullingCompute->setInt("depthMap", 12);
	LightCullingCompute->setFloat("far", scene->getMainCamera()->farPlane);
	LightCullingCompute->setFloat("near", scene->getMainCamera()->nearPlane);
	LightCullingCompute->dispatch(workGroupsX, workGroupsY);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// final render pass
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE); // Disable depth write for the main render pass
	sceneRenderer.renderScene(scene);
	glDepthMask(GL_TRUE);
	HDRBBuffer.unBind();

	toneMappingPass();

	if (debug) {
		LightCullingDebugShader->use();
		LightCullingDebugShader->setFloat("far", scene->getMainCamera()->farPlane);
		LightCullingDebugShader->setFloat("near", scene->getMainCamera()->nearPlane);
		LightCullingDebugShader->setInt("depthMap", 12);
		LightCullingDebugShader->setInt("numberOfTilesX", workGroupsX);
		LightCullingDebugShader->setInt("totalLightCount", scene->getPointLIghts().size());
		glBindVertexArray(screenQuadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	HDRBBuffer.bind(GL_READ_FRAMEBUFFER);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void ForwardRenderer::toneMappingPass()
{
	basicToneMappingShader->setInt("hdrBuffer", 11);
	basicToneMappingShader->setFloat("exposure", 0.6f);
	HDRBUfferTexture->bind(GL_TEXTURE0 + 11);
	basicToneMappingShader->use();
	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}