#pragma once

#ifndef SCRIPT_H
#define SCRIPT_H

#include "Globals.h"

class Entity;

class Script
{
protected:

	Entity* parent;

public:
	void attachParentEntity(Entity* entity);
	virtual void GLN_API Start();
	virtual void GLN_API Update(float deltaTime); // TODO: ADD Delta time 
};

#endif // !SCRIPT_H