#pragma once

#ifndef SCRIPT_H
#define SCRIPT_H

#include "Glen/Globals.h"

class Entity;

class Script
{
protected:

	Entity* parent;

public:
	void attachParentEntity(Entity* entity) { parent = entity; }
	virtual void GLN_API Start() {}
	virtual void GLN_API Update(float deltaTime) {}
};

#endif // !SCRIPT_H