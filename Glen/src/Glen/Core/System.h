#pragma once

#ifndef SYSTEM_H

#include <string>
#include "Glen/Globals.h"
#include "Glen/Utils/TypeName.h"

class ISystem {
public:
	virtual void startup() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void shutdown() = 0;

	ISystem() = default;
	std::string name;
};

class SystemManager {
public:
	template<typename T>
	void registerSystem();
	void initialize();
	void shutdown();

private:
	std::vector<ISystem*> systems;
};

template<typename T>
void SystemManager::registerSystem(){
	T* system = new T();
	ISystem* ref = static_cast<ISystem*>(system);
	//system->name = TypeName<T>();
	systems.push_back(ref);
}

#endif // !SYSTEM_H
