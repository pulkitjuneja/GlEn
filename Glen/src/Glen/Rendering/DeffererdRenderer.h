#pragma once

#ifndef DEFFEREDRENDERER_H
#define DEFFEREDRENDERER_H

#define G_BUFFER_POSITION_TEXTURE_NAME "gb_position_texture"
#define G_BUFFER_NORMAL_TEXTURE_NAME "gb_normal_texture"
#define G_BUFFER_COLOR_TEXTURE_NAME "gb_color_texture"
#define G_BUFFER_DEPTH_TEXTURE_NAME "gb_depth_texture"
#define G_BUFFER_VELOCITY_TEXTURE_NAME "gb_velocity_texture"
#define HDR_BUFFER_TEXTURE_NAME "hdr_buffer_texture"
#define PP_BUFFER_TEXTURE_NAME "pp_buffer_texture"

#include "Texture.h"
#include "FrameBuffer.h"
#include "Glen/ResourceManagement/ResourceManager.h"
#include "Uniforms.h"
#include "SceneRenderer.h"
#include "Buffer.h"
#include "Csm.h"
#include <array>
#include "Glen/Core/System.h"
#include "TAA.h"

// TODO: Split TAA functionality intoa different class
class DefferedRenderer : public ISystem {
	SceneManager* scene;

	std::array<Texture2D*, 5> gBufferTextures;
	Texture2D* HDRBUfferTexture;
	Texture2D* postProcessingTexture;
	CubeMap* skybox;
	CubeMap* irradianceMap;
	CubeMap* preFilterMap;

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
	Buffer TAAUbo;

	SceneRenderer sceneRenderer;
	Csm csm;
	TAA taa;

	GLuint screenQuadVAO;
	Mesh* pointVolumeMesh;

	Shader* directionalLightShader;
	Shader* pointLightShader;
	Shader* basicToneMappingShader;
	Shader* ssr;
	Shader* textureDebugShader;

	bool skyboxCheck = true;

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