#pragma once

#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "Glen/Scene/Scene.h"
#include "Glen/Globals.h"
#include "Glen/ResourceManagement/ResourceManager.h"
#include "Uniforms.h"

class SceneRenderer {
public:
	void setGlobalUniforms(PerFrameUniforms& perFrameUniforms, Scene* scene);
	void renderScene(Scene* scene, Material* overrideMaterial = nullptr, bool passBaseMaterialProperties = false);
	void bindGlobalMaps();
};

#endif // !SCENERENDERER_H
