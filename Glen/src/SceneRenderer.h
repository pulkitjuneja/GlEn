#pragma once

#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "Scene.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "Uniforms.h"

class SceneRenderer {
public:
	void setGlobalUniforms(PerFrameUniforms& perFrameUniforms, Scene* scene);
	void renderScene(Scene* scene, Material* overrideMaterial = nullptr, bool passBaseMaterialProperties = false);
	void bindGlobalMaps();
};

#endif // !SCENERENDERER_H
