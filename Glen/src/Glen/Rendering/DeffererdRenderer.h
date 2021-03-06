#pragma once

#ifndef DEFFEREDRENDERER_H
#define DEFFEREDRENDERER_H

#define G_BUFFER_POSITION_TEXTURE_NAME "gb_position_texture"
#define G_BUFFER_NORMAL_TEXTURE_NAME "gb_normal_texture"
#define G_BUFFER_COLOR_TEXTURE_NAME "gb_color_texture"
#define G_BUFFER_DEPTH_TEXTURE_NAME "gb_depth_texture"
#define HDR_BUFFER_TEXTURE_NAME "hdr_buffer_texture"
#define PP_BUFFER_TEXTURE_NAME "pp_buffer_texture"

#include "Texture.h"
#include "FrameBuffer.h"
#include "Glen/ResourceManagement/ResourceManager.h"
#include "Uniforms.h"
#include "SceneRenderer.h"
#include "Buffer.h"
#include "Csm.h"
#include "Glen/Core/System.h"

class DefferedRenderer : public ISystem {
	SceneManager* scene;

	Texture2D* gBufferPBRInfoTexture;
	Texture2D* gBufferColorTexture;
	Texture2D* gBufferNormalTexture;
	Texture2D* gBUfferDepthTexture;
	Texture2D* HDRBUfferTexture;
	Texture2D* postProcessingTexture;
	CubeMap* skybox;

	FrameBuffer HDRBBuffer;
	FrameBuffer postProcessingBuffer;
	FrameBuffer gBuffer;

	//Uniforms
	PerFrameUniforms perFrameUniforms;
	CSMUniforms csmUniforms;

	//UniformBUffers
	Buffer perFrameUbo;
	Buffer CsmUbo;
	Buffer* pointLightBuffer;

	SceneRenderer sceneRenderer;
	Csm csm;

	GLuint screenQuadVAO;
	Mesh* pointVolumeMesh;

	Shader* directionalLightShader;
	Shader* pointLightShader;
	Shader* basicToneMappingShader;
	Shader* ssr;

	void createUVSphere();
	void setupGBuffer();
	void setupHDRBuffer();
public:
	DefferedRenderer();
	void toneMappingPass();
	void runGeometryPass();
	void runDirectionalLightPass();
	void runPointLightPass();

	virtual void startup() override;
	virtual void update(float deltaTimer) override;
	virtual void shutdown() override;
};

#endif