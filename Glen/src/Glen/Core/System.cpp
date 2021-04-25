#include "System.h"

void SystemManager::initialize()
{
	for (auto system : systems) {
		system->startup();
	}
}

void SystemManager::update(float deltaTime)
{
	for (auto system : systems) {
		system->update(deltaTime);
	}
}

void SystemManager::shutdown()
{
	for (auto system : systems) {
		system->shutdown();
	}

	systems.clear();
}


