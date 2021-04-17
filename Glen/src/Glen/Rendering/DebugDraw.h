#pragma once

#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <GL/glew.h>

#include "Glen/Core/Core.h"
#include "Glen/Globals.h"
#include "Glen/Rendering/Mesh.h"
#include "Glen/ResourceManagement/ResourceManager.h"

class Scene;

class GLN_API DebugDraw {
private:
	Mesh* DebugCubeMesh;
public:

	DebugDraw();

	// TODO : Dont know if these functions belong here, re-evaluate
	// Debug mesh creators
	void createDebugMesh();
	void createSphereMesh() {};
	void createCylinderMesh() {};

	void render(Scene* scene);
};

#endif // !DEBUG_DRAW_H
