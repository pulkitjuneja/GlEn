#pragma once 


#ifndef SCRIPTING_SYSTEM

#include "Glen/Core/System.h"
#include "Glen/Scene/Scene.h"

class ScriptingSystem : public ISystem {
public:
	virtual void startup() override;
	virtual void update(float deltaTime) override;
	virtual void shutdown() override;
};

#endif // !SCRIPTING_SYSTEM
