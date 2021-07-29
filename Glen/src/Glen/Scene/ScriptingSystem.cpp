#include "ScriptingSystem.h"

void ScriptingSystem::startup()
{
	// Initialize any dependencies for the script
}

void ScriptingSystem::update(float deltaTime)
{
	std::vector<Entity>& entities = EngineContext::get()->sceneManager->getEntities();
	for (int i = 0; i < entities.size(); i++) {
		Entity& ent = entities[i];
		// Store previous Model Matrix before updates
		// TODO: move this to a better place
		ent.prevModelMatrix = ent.getTransform()->getTransformationMatrix();

		for (auto script : ent.scripts) {
			script->Update(deltaTime);
		}
	}
}

void ScriptingSystem::shutdown() {}
