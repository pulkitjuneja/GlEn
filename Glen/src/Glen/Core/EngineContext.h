#pragma once


#ifndef ENGINE_CONTEXT_H

#include "Core.h"
#include <vector>

class ResourceManager;
class Window;
class InputStatus;
class SceneManager;
class PhysicsContext;
class IAllocator;
class IPanel;

struct engineStats {
	float lastFrameTime;
	bool isFirstFame = true;
};


class GLN_API EngineContext {
public:
	ResourceManager* resourceManager = nullptr;
	Window* window = nullptr;
	InputStatus* inputStatus = nullptr;
	SceneManager* sceneManager = nullptr;
	PhysicsContext* physicsContext = nullptr;
	IAllocator* sceneAllocator = nullptr;
	std::vector<IPanel*> UIPanels;
	engineStats stats;

	static EngineContext* instance;
	static EngineContext* get();
private:
	EngineContext() = default;
};

#endif // !ENGINE_CONTEXT_H
