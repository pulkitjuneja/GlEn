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
public:
	// TODO : Dont know if these functions belong here, re-evaluate
	// Debug mesh creators
	static Mesh* createCubeMesh();
	static Mesh* createSphereMesh() { return nullptr; };
	static Mesh* createCylinderMesh() { return nullptr; };

	void render(Scene* scene);
};

#endif // !DEBUG_DRAW_H
