#pragma once

#ifndef SCRIPT_H
#define SCRIPT_H

#include "Glen/Globals.h"

class Entity;
class SceneManager;

class Script
{
friend Entity;
protected:

	Entity* parent;
	SceneManager* scene;

public:
	void attachParentEntity(Entity* entity) { parent = entity; }
	virtual void GLN_API Start() {}
	virtual void GLN_API Update(float deltaTime) {}

	virtual void GLN_API onCollisionBegin(Entity& other) {};
	virtual void GLN_API onCollisionEnd(Entity& other) {};
};

#endif // !SCRIPT_H