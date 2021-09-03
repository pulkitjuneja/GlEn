#include "DeffererdRenderer.h"
#include "Glen/Core/Input.h"


DefferedRenderer::DefferedRenderer() : csm(0.3, 150.0f, 3, 4096), 
	perFrameUbo(sizeof(PerFrameUniforms), 0, BufferType::UBO), CsmUbo(sizeof(CSMUniforms), 1, BufferType::UBO){}

void DefferedRenderer::createUVSphere()
{
	int stacks = 10;
	int slices = 10;
	const float PI = 3.14f;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	// loop through stacks.
	for (int i = 0; i <= stacks; ++i) {

		float V = (float)i / (float)stacks;
		float phi = V * PI;

		// loop through the slices.
		for (int j = 0; j <= slices; ++j) {

			float U = (float)j / (float)slices;
			float theta = U * (PI * 2);

			// use spherical coordinates to calculate the positions.
			float x = cos(theta) * sin(phi);
			float y = cos(phi);
			float z = sin(theta) * sin(phi);
			Vertex vertex;
			vertex.position = glm::vec3(x, y, z);
			vertices.push_back(vertex);
		}
	}

	// Calc The Index Positions
	for (int i = 0; i < slices * stacks + slices; ++i) {
		indices.push_back(GLuint(i));
		indices.push_back(GLuint(i + slices + 1));
		indices.push_back(GLuint(i + slices));

		indices.push_back(GLuint(i + slices + 1));
		indices.push_back(GLuint(i));
		indices.push_back(GLuint(i + 1));
	}
	std::vector<SubMesh> submeshes;
	submeshes.resize(1);
	submeshes[0].indexCount = indices.size();
	pointVolumeMesh = EngineContext::get()->resourceManager->CreateMesh("UV_SPHERE", vertices, indices, submeshes, false, false, false);
}

void DefferedRenderer::setupGBuffer()
{
	gBuffer.bind();

	gBufferNormalTexture = EngineContext::get()->resourceManager->generateTexture(G_BUFFER_NORMAL_TEXTURE_NAME, TextureType::RENDERTEXTURE,
		SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_RGBA16F, GL_FLOAT);
	gBufferNormalTexture->bind();
	gBufferNormalTexture->setMinMagFilter(GL_NEAREST, GL_NEAREST);
	gBuffer.attachRenderTarget(gBufferNormalTexture, 0, 0);

	gBufferColorTexture = EngineContext::get()->resourceManager->generateTexture(G_BUFFER_COLOR_TEXTURE_NAME, TextureType::RENDERTEXTURE,
		SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	gBufferColorTexture->bind();
	gBufferColorTexture->setMinMagFilter(GL_NEAREST, GL_NEAREST);
	gBuffer.attachRenderTarget(gBufferColorTexture, 0, 1);

	gBufferPBRInfoTexture = EngineContext::get()->resourceManager->generateTexture(G_BUFFER_POSITION_TEXTURE_NAME, TextureType::RENDERTEXTURE,
	SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_RGBA, GL_FLOAT);
	gBufferPBRInfoTexture->bind();
	gBufferPBRInfoTexture->setMinMagFilter(GL_NEAREST, GL_NEAREST);
	gBuffer.attachRenderTarget(gBufferPBRInfoTexture, 0, 2);

	gBUfferDepthTexture = EngineContext::get()->resourceManager->generateTexture(G_BUFFER_DEPTH_TEXTURE_NAME, TextureType::DEPTH, SCREEN_WIDTH, SCREEN_HEIGHT,
		GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	gBUfferDepthTexture->bind();
	gBUfferDepthTexture->setMinMagFilter(GL_LINEAR, GL_LINEAR);
	gBUfferDepthTexture->setWrapping(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	gBuffer.attachDepthTarget(gBUfferDepthTexture, 0);
	gBuffer.checkStatus();
	gBuffer.unBind();
}

void DefferedRenderer::setupHDRBuffer()
{
	HDRBBuffer.bind();

	HDRBUfferTexture = EngineContext::get()->resourceManager->generateTexture(HDR_BUFFER_TEXTURE_NAME, TextureType::RENDERTEXTURE,
		SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_RGBA16F, GL_FLOAT);
	HDRBUfferTexture->bind();
	HDRBUfferTexture->setMinMagFilter(GL_NEAREST, GL_NEAREST);
	HDRBBuffer.attachRenderTarget(HDRBUfferTexture, 0, 0);
	HDRBBuffer.checkStatus();
	HDRBBuffer.unBind();

	postProcessingBuffer.bind();
	postProcessingTexture = EngineContext::get()->resourceManager->generateTexture(PP_BUFFER_TEXTURE_NAME, TextureType::RENDERTEXTURE,
		SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_RGBA, GL_FLOAT);
	postProcessingTexture->bind();
	postProcessingTexture->setMinMagFilter(GL_NEAREST, GL_NEAREST);
	postProcessingBuffer.attachRenderTarget(postProcessingTexture, 0, 0);
	postProcessingBuffer.checkStatus();
	postProcessingBuffer.unBind();

}

void DefferedRenderer::startup()
{
	setupGBuffer();
	setupHDRBuffer();

	directionalLightShader = EngineContext::get()->resourceManager->getShader("defferedDirectionalLightPass");
	pointLightShader = EngineContext::get()->resourceManager->getShader("defferedPointLightPass");
	basicToneMappingShader = EngineContext::get()->resourceManager->getShader("basicToneMapping");
	pointLightBuffer = Mem::Allocate<Buffer>(sizeof(PointLight) * MAXPOINTLIGHTS, 5, BufferType::SSBO);

	ssr = EngineContext::get()->resourceManager->getShader("ssrPass");
	std::vector<std::string> facePaths = {
			"right.jpg",
			"left.jpg",
			"top.jpg",
			"bottom.jpg",
			"front.jpg",
			"back.jpg"
	};

	skybox  = EngineContext::get()->resourceManager->loadHdriMap("Arches_E_PineTree_3k.hdr", "Assets/Textures/Arches_E_PineTree");
	irradianceMap = skybox->createConvolutionMap("SKYBOX_IRRADIANCE");
	preFilterMap = skybox->createPrefilteredEnvMap("SKYBOX_PFM");
	
	directionalLightShader->setInt("normalTexture", 11);
	directionalLightShader->setInt("albedoTexture", 12);
	directionalLightShader->setInt("depthTexture", 13);
	directionalLightShader->setInt("shadowMap", 10);
	directionalLightShader->setInt("PBRInfoTexture", 14);
	directionalLightShader->setInt("skybox", 16);
	directionalLightShader->setInt("irradianceMap", 19);
	directionalLightShader->setInt("prefilterMap", 20);

	pointLightShader->setInt("normalTexture", 11);
	pointLightShader->setInt("albedoTexture", 12);
	pointLightShader->setInt("depthTexture", 13);
	pointLightShader->setInt("PBRInfoTexture", 14);
	pointLightShader->setInt("irradianceMap", 19);
	pointLightShader->setInt("prefilterMap", 20);

	skybox->bind(GL_TEXTURE0 + 16);
	irradianceMap->bind(GL_TEXTURE0 + 19);
	preFilterMap->bind(GL_TEXTURE0 + 20);

	ssr->setInt("normalTexture", 11);
	ssr->setInt("albedoTexture", 12);

	// Create an empoty VAO to be bound when rendering screen quad
	glGenVertexArrays(1, &screenQuadVAO);
	createUVSphere();

	std::cout << glGetError() << std::endl;
	scene = EngineContext::get()->sceneManager;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void DefferedRenderer::runGeometryPass()
{
	gBuffer.bind();

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Material* gBufferMaterial = new Material();
	gBufferMaterial->setShader(EngineContext::get()->resourceManager->getShader("defferedGeometryPass"));
	sceneRenderer.renderScene(scene, gBufferMaterial, true);
	gBuffer.unBind();

	gBufferNormalTexture->bind(GL_TEXTURE0 + 11);
	gBufferColorTexture->bind(GL_TEXTURE0 + 12);
	gBUfferDepthTexture->bind(GL_TEXTURE0 + 13);
	gBufferPBRInfoTexture->bind(GL_TEXTURE0 + 14);

}

void DefferedRenderer::runDirectionalLightPass()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	directionalLightShader->use();
	directionalLightShader->setBool("skyBoxCheck", skyboxCheck);
	//render simple quad
	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void DefferedRenderer::runPointLightPass()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glFrontFace(GL_CW);
	pointLightShader->use();

	glBindVertexArray(pointVolumeMesh->VAO);

	glDrawElementsInstanced(GL_TRIANGLES, pointVolumeMesh->subMeshes[0].indexCount, GL_UNSIGNED_INT, 0, scene->getPointLIghts().size());

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glFrontFace(GL_CCW);
}

void DefferedRenderer::update(float deltaTime)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	csm.update(EngineContext::get()->sceneManager->getMainCamera(), scene->getDirectionalLight().direction);

	csm.updateUniforms(csmUniforms);
	sceneRenderer.setGlobalUniforms(perFrameUniforms, scene);
	pointLightBuffer->setData(&scene->getPointLIghts()[0], sizeof(PointLight) * scene->getPointLIghts().size(), true);

	perFrameUbo.setData(&perFrameUniforms, sizeof(perFrameUniforms), true);
	CsmUbo.setData(&csmUniforms, sizeof(csmUniforms), true);

	csm.render(scene);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	runGeometryPass();

	HDRBBuffer.bind();
	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);
	runDirectionalLightPass();
	runPointLightPass();
	HDRBBuffer.unBind();

	toneMappingPass();

	gBuffer.bind(GL_READ_FRAMEBUFFER);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	InputStatus* st = EngineContext::get()->inputStatus;
	if (st->isKeyPressed(Keys::X)) {
		skyboxCheck = !skyboxCheck;
	}
	/*postProcessingTexture->bind(GL_TEXTURE0 + 15);
	ssr->setInt("finalImageBuffer", 15);
	postProcessingBuffer->unBind();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ssr->use();

	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);*/

	//toneMappingPass();
}

void DefferedRenderer::shutdown() {}

void DefferedRenderer::toneMappingPass()
{
	basicToneMappingShader->setInt("hdrBuffer", 15);
	basicToneMappingShader->setFloat("exposure", 0.7f);
	HDRBUfferTexture->bind(GL_TEXTURE0 + 15);
	basicToneMappingShader->use();
	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

