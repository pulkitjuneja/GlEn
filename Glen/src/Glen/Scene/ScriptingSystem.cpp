#include "ScriptingSystem.h"

void ScriptingSystem::startup()
{
	// Initialize any dependencies for the script
}

void ScriptingSystem::update(float deltaTime)
{
	std::vector<Entity> entities = EngineContext::get()->sceneManager->getEntities();
	std::vector<Entity>::iterator it = entities.begin();
	for (; it != entities.end(); it++) {
		for (auto script : (*it).scripts) {
			script->Update(deltaTime);
		}
	}
}

void ScriptingSystem::shutdown() {}
