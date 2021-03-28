#pragma once

#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include "Globals.h"
#include "Mesh.h"
#include "ResourceManager.h"

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
