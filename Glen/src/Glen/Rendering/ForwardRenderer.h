#pragma once 

#ifndef RENDERER_H
#define RENDERER_H

#include "SceneRenderer.h"
#include "Uniforms.h"
#include "UniformBuffer.h"
#include "Glen/Scene/Scene.h"
#include "CSM.h"
#include "Glen/Constants.h"

#define HDR_BUFFER_TEXTURE_NAME "hdr_buffer_texture"
#define DEPTH_TEXTURE_NAME "depth_texture"

class GLN_API ForwardRenderer : public ISystem {
	//Uniforms
	PerFrameUniforms perFrameUniforms;
	CSMUniforms csmUniforms;
	Csm* csm;

	Texture2D* HDRBUfferTexture;
	Texture2D* depthTexture;
	FrameBuffer HDRBBuffer;
	Shader* basicToneMappingShader;
	GLuint screenQuadVAO;

	//UniformBUffers
	Buffer* perFrameUbo;
	Buffer* CsmUbo;

	SceneManager* scene;
	SceneRenderer sceneRenderer;

public:
	ForwardRenderer();
	void toneMappingPass();
	void setupHDRBuffer();

	virtual void startup() override;
	virtual void update(float deltaTimer) override;
	virtual void shutdown() override;
};

#endif // !RENDERER_H

