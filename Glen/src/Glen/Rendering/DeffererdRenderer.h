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
#include "UniformBuffer.h"
#include "Csm.h"

class DefferedRenderer {
	Scene* scene;

	Texture* gBufferPositionTexture;
	Texture* gBufferColorTexture;
	Texture* gBufferNormalTexture;
	Texture* gBUfferDepthTexture;
	Texture* HDRBUfferTexture;
	Texture* postProcessingTexture;
	FrameBuffer* HDRBBuffer;
	FrameBuffer* postProcessingBuffer;
	FrameBuffer* gBuffer;

	//Uniforms
	PerFrameUniforms perFrameUniforms;
	CSMUniforms csmUniforms;

	//UniformBUffers
	UniformBuffer* perFrameUbo;
	UniformBuffer* CsmUbo;

	SceneRenderer sceneRenderer;
	Csm* csm;

	GLuint screenQuadVAO;
	Mesh* pointVolumeMesh;

	Shader* directionalLightShader;
	Shader* pointLightShader;
	Shader* basicToneMappingShader;
	Shader* ssr;

	DebugDraw debugDraw;

	void createUVSphere();
	void setupGBuffer();
	void setupHDRBuffer();
public:
	DefferedRenderer();
	void toneMappingPass();
	void runGeometryPass();
	void runDirectionalLightPass();
	void runPointLightPass();
	void setScene(Scene* scene);
	//void runLightingPass();
	void render();
};

#endif