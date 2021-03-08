#pragma once 

#ifndef RENDERER_H
#define RENDERER_H

#include "SceneRenderer.h"
#include "Uniforms.h"
#include "UniformBuffer.h"
#include "Scene.h"

class ForwardRenderer {
	//Uniforms
	PerFrameUniforms perFrameUniforms;

	//UniformBUffers
	UniformBuffer* perFrameUbo;

public:
	ForwardRenderer();
	SceneRenderer sceneRenderer;
	void render(Scene* scene);
};

#endif // !RENDERER_H

