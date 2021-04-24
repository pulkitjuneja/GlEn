#pragma once 

#ifndef RENDERER_H
#define RENDERER_H

#include "SceneRenderer.h"
#include "Uniforms.h"
#include "UniformBuffer.h"
#include "Glen/Scene/Scene.h"
#include "CSM.h"
#include "DebugDraw.h"

class GLN_API ForwardRenderer {
	//Uniforms
	PerFrameUniforms perFrameUniforms;
	CSMUniforms csmUniforms;

	//UniformBUffers
	UniformBuffer* perFrameUbo;
	UniformBuffer* CsmUbo;

public:
	ForwardRenderer();
	SceneRenderer sceneRenderer;
	Csm* csm;
	void render(SceneManager* scene);
};

#endif // !RENDERER_H

