#pragma once 

#ifndef RENDERER_H
#define RENDERER_H

#include "SceneRenderer.h"
#include "Uniforms.h"
#include "Buffer.h"
#include "Glen/Scene/Scene.h"
#include "CSM.h"
#include "Glen/Constants.h"

#define HDR_BUFFER_TEXTURE_NAME "hdr_buffer_texture"
#define DEPTH_TEXTURE_NAME "depth_texture"
#define MAX_LIGHTS_PER_TILE 1024

class GLN_API ForwardRenderer : public ISystem {
	//Uniforms
	PerFrameUniforms perFrameUniforms;
	CSMUniforms csmUniforms;
	Csm* csm;

	Texture2D* HDRBUfferTexture;
	Texture2D* depthTexture;
	FrameBuffer HDRBBuffer;
	GLuint screenQuadVAO;

	//UniformBUffers
	Buffer* perFrameUbo;
	Buffer* CsmUbo;
	Buffer* visibleLightBuffer;
	Buffer* pointLightBuffer;
	// temp remove
	Buffer* DebugDepthBuffer;

	Material depthPassMaterial;
	Shader* basicToneMappingShader;
	ComputeShader* LightCullingCompute;
	Shader* LightCullingDebugShader;

	SceneManager* scene;
	SceneRenderer sceneRenderer;

	bool debug;

public:
	ForwardRenderer();
	void toneMappingPass();
	void setupHDRBuffer();

	virtual void startup() override;
	virtual void update(float deltaTimer) override;
	virtual void shutdown() override;
};

#endif // !RENDERER_H

