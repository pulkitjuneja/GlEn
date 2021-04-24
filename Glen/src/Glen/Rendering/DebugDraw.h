#pragma once

#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <GL/glew.h>

#include "Glen/Core/Core.h"
#include "Glen/Globals.h"
#include "Glen/Rendering/Mesh.h"
#include "Glen/ResourceManagement/ResourceManager.h"
#include "Glen/Core/EngineContext.h"
#include "Glen/Core/System.h"

class SceneManager;

class GLN_API DebugDraw : ISystem {
private:
	Mesh* DebugCubeMesh;
	SceneManager* scene;

public:

	DebugDraw() = default;

	// TODO : Dont know if these functions belong here, re-evaluate
	// Debug mesh creators
	void createDebugMesh();
	void createSphereMesh() {};
	void createCylinderMesh() {};

	void startup() override;
	void update(float deltaTimer) override;
	void shutdown() override;
};

#endif // !DEBUG_DRAW_H
