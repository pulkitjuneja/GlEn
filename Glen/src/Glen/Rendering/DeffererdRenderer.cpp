#include "DeffererdRenderer.h"
#include <imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"
#include "Glen/Core/Window.h"
#include "Glen/Utils/HaltonSequence.h"

DefferedRenderer::DefferedRenderer() : csm(0.3, 150.0f, 3, 4096), 
	perFrameUbo(sizeof(PerFrameUniforms), 0, BufferType::UBO), 
	CsmUbo(sizeof(CSMUniforms), 1, BufferType::UBO),
	TAAUbo(sizeof(TAAUbo),2, BufferType::UBO){}

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

	gBufferTextures[2] = EngineContext::get()->resourceManager->generateTexture(G_BUFFER_NORMAL_TEXTURE_NAME, TextureType::RENDERTEXTURE,
		SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_RGBA16F, GL_FLOAT);
	gBufferTextures[2]->bind();
	gBufferTextures[2]->setMinMagFilter(GL_NEAREST, GL_NEAREST);
	gBuffer.attachRenderTarget(gBufferTextures[2], 0, 0);

	gBufferTextures[1] = EngineContext::get()->resourceManager->generateTexture(G_BUFFER_COLOR_TEXTURE_NAME, TextureType::RENDERTEXTURE,
		SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	gBufferTextures[1]->bind();
	gBufferTextures[1]->setMinMagFilter(GL_NEAREST, GL_NEAREST);
	gBuffer.attachRenderTarget(gBufferTextures[1], 0, 1);

	gBufferTextures[0] = EngineContext::get()->resourceManager->generateTexture(G_BUFFER_POSITION_TEXTURE_NAME, TextureType::RENDERTEXTURE,
	SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_RGBA, GL_FLOAT);
	gBufferTextures[0]->bind();
	gBufferTextures[0]->setMinMagFilter(GL_NEAREST, GL_NEAREST);
	gBuffer.attachRenderTarget(gBufferTextures[0], 0, 2);

	gBufferTextures[3] = EngineContext::get()->resourceManager->generateTexture(G_BUFFER_DEPTH_TEXTURE_NAME, TextureType::DEPTH, SCREEN_WIDTH, SCREEN_HEIGHT,
		GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	gBufferTextures[3]->bind();
	gBufferTextures[3]->setMinMagFilter(GL_LINEAR, GL_LINEAR);
	gBufferTextures[3]->setWrapping(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	gBufferTextures[4] = EngineContext::get()->resourceManager->generateTexture(G_BUFFER_VELOCITY_TEXTURE_NAME, TextureType::RENDERTEXTURE,
		SCREEN_WIDTH, SCREEN_HEIGHT, GL_RG, GL_RG16F, GL_FLOAT);
	gBufferTextures[4]->bind();
	gBufferTextures[4]->setMinMagFilter(GL_LINEAR, GL_LINEAR);
	gBufferTextures[4]->setWrapping(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	gBuffer.attachRenderTarget(gBufferTextures[4], 0, 3);

	gBuffer.attachDepthTarget(gBufferTextures[3], 0);
	gBuffer.checkStatus();
	gBuffer.unBind();
}

void DefferedRenderer::setupHDRBuffer()
{
	HDRBBuffer.bind();

	HDRBUfferTexture = EngineContext::get()->resourceManager->generateTexture(HDR_BUFFER_TEXTURE_NAME, TextureType::RENDERTEXTURE,
		SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_RGBA16F, GL_FLOAT);
	HDRBUfferTexture->bind();
	HDRBUfferTexture->setMinMagFilter(GL_LINEAR, GL_LINEAR);
	HDRBUfferTexture->setWrapping(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
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
	textureDebugShader = EngineContext::get()->resourceManager->getShader("TextureDebugShader");
	TAAPass = EngineContext::get()->resourceManager->getShader("TAA");

	ssr = EngineContext::get()->resourceManager->getShader("ssrPass");
	std::vector<std::string> facePaths = {
			"right.jpg",
			"left.jpg",
			"top.jpg",
			"bottom.jpg",
			"front.jpg",
			"back.jpg"
	};

	skybox = EngineContext::get()->resourceManager->loadHdriMap("Arches_E_PineTree_3k.hdr", "Assets/Textures/Arches_E_PineTree");
	
	directionalLightShader->setInt("normalTexture", 11);
	directionalLightShader->setInt("albedoTexture", 12);
	directionalLightShader->setInt("depthTexture", 13);
	directionalLightShader->setInt("shadowMap", 10);
	directionalLightShader->setInt("PBRInfoTexture", 14);

	pointLightShader->setInt("normalTexture", 11);
	pointLightShader->setInt("albedoTexture", 12);
	pointLightShader->setInt("depthTexture", 13);
	pointLightShader->setInt("PBRInfoTexture", 14);

	ssr->setInt("normalTexture", 11);
	ssr->setInt("albedoTexture", 12);

	// Create an empoty VAO to be bound when rendering screen quad
	glGenVertexArrays(1, &screenQuadVAO);
	createUVSphere();

	std::cout << glGetError() << std::endl;
	scene = EngineContext::get()->sceneManager;
	
	// TAA initialization stuff
	initializeTAAFbo(0);
	initializeTAAFbo(1);
	createJitterArray();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void DefferedRenderer::runGeometryPass()
{
	gBuffer.bind();

	unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Material* gBufferMaterial = new Material();
	gBufferMaterial->setShader(EngineContext::get()->resourceManager->getShader("defferedGeometryPass"));
	sceneRenderer.renderScene(scene, gBufferMaterial, true);
	gBuffer.unBind();

	gBufferTextures[2]->bind(GL_TEXTURE0 + 11); //Normal Map
	gBufferTextures[1]->bind(GL_TEXTURE0 + 12); // diffuse color
	gBufferTextures[3]->bind(GL_TEXTURE0 + 13); // depth Texture
	gBufferTextures[0]->bind(GL_TEXTURE0 + 14); // PBR info
	gBufferTextures[4]->bind(GL_TEXTURE0 + 17); // velocity texture

}

void DefferedRenderer::runDirectionalLightPass()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox->bind(GL_TEXTURE0 + 16);
	directionalLightShader->setInt("skybox", 16);

	directionalLightShader->use();

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

	if (EngineContext::get()->stats.isFirstFame) {
		scene->prevViewMatrix = scene->getMainCamera()->getViewMatrix();
		scene->prevProjectionMatrix = scene->getMainCamera()->getProjectionMatrix();
	}

	glm::vec2 pixelSize(1 / float(SCREEN_WIDTH), 1 / (SCREEN_HEIGHT));
	glm::vec2 jitterOffset = jitterArray[jitterIndex] * pixelSize;
	glm::mat4 jitteredProjectionMat = scene->getMainCamera()->getProjectionMatrix();
	jitteredProjectionMat[2][0] += jitterOffset.x;
	jitteredProjectionMat[2][1] += jitterOffset.y;
	jitterIndex = (jitterIndex + 1) % 16;

	taaUniforms.jitteredProjMatrix = jitteredProjectionMat;
	taaUniforms.inverseJitteredProjMatrix = glm::inverse(jitteredProjectionMat);
	taaUniforms.prevViewMatrix = scene->prevViewMatrix;
	taaUniforms.prevProjectionMatrix = scene->prevProjectionMatrix;
	taaUniforms.jitter = jitterOffset;
	taaUniforms.feedback = 0.75;
	scene->prevViewMatrix = scene->getMainCamera()->getViewMatrix();
	scene->prevProjectionMatrix = scene->getMainCamera()->getProjectionMatrix();

	pointLightBuffer->setData(&scene->getPointLIghts()[0], sizeof(PointLight) * scene->getPointLIghts().size(), true);
	perFrameUbo.setData(&perFrameUniforms, sizeof(perFrameUniforms), true);
	CsmUbo.setData(&csmUniforms, sizeof(csmUniforms), true);
	TAAUbo.setData(&taaUniforms, sizeof(taaUniforms), true);

	csm.render(scene);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	runGeometryPass();

	int activeFBO = flip  ? 0 : 1;
	HDRBBuffer.bind();
	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);
	runDirectionalLightPass();
	runPointLightPass();
	HDRBBuffer.unBind();
	HDRBUfferTexture->bind(GL_TEXTURE0 + 15);

	if (EngineContext::get()->stats.isFirstFame) {
		EngineContext::get()->stats.isFirstFame = false;
		return;
	}

	runTAAPass(activeFBO);
	aaRenderTextures[activeFBO]->bind(GL_TEXTURE0 + 18);
	toneMappingPass();

	gBuffer.bind(GL_READ_FRAMEBUFFER);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	//if (EngineContext::get()->stats.isFirstFame)
	//	EngineContext::get()->stats.isFirstFame = false;

	 //------------------------  Temp setup to debug textures ---------------------------
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Add UI panels here
	static bool show = true;

	ImGui::Begin("Velocity buffer");
	ImGui::BeginChild("Texture");

	ImVec2 wsize = ImGui::GetWindowSize();
	ImGui::Image((ImTextureID)aaRenderTextures[1 - activeFBO]->textureId, wsize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::EndChild();
	ImGui::End();

	ImGuiIO& io = ImGui::GetIO();
	Window* window = EngineContext::get()->window;
	io.DisplaySize = ImVec2(window->getWindowData().width, window->getWindowData().height);

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
	 //--------------------------------- Ends Here --------------------------------------
}

void DefferedRenderer::shutdown() {}

void DefferedRenderer::createJitterArray()
{
	for (int i = 0; i < 16; i++) {
		jitterArray[i] = glm::vec2(CreateHaltonSequence(i + 1, 2), CreateHaltonSequence(i + 1, 3));
	}
}

void DefferedRenderer::initializeTAAFbo(int fboId)
{
	aaFbos[fboId].bind();

	aaRenderTextures [fboId] = EngineContext::get()->resourceManager->generateTexture(TAA_COLOR_TEXTURE(id), TextureType::RENDERTEXTURE,
		SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_RGBA16F, GL_FLOAT);
	aaRenderTextures[fboId]->bind();
	aaRenderTextures[fboId]->setMinMagFilter(GL_LINEAR, GL_LINEAR);
	aaRenderTextures[fboId]->setWrapping(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	aaFbos[fboId].attachRenderTarget(aaRenderTextures[fboId], 0, 0);

	aaFbos[fboId].checkStatus();
	aaFbos[fboId].unBind();
}

void DefferedRenderer::runTAAPass(int activeFBO)
{
	// ------------------- THIS IF CONDITION IS THE ASSHOLE HERE ------------------
	if (EngineContext::get()->stats.frameCount == 1) {
		HDRBBuffer.bind(GL_READ_FRAMEBUFFER);
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
		unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);

		TAAPass->use();
		glBindVertexArray(screenQuadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		aaFbos[activeFBO].unBind();
	}


	flip = !flip;
}

void DefferedRenderer::toneMappingPass()
{
	basicToneMappingShader->setInt("hdrBuffer", 18);
	basicToneMappingShader->setFloat("exposure", 0.7f);
	basicToneMappingShader->use();
	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

