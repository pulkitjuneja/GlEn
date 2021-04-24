#include "EngineContext.h"

EngineContext* EngineContext::instance;

EngineContext* EngineContext::get()
{
	if (instance == nullptr) {
		instance = new EngineContext();
	}

	return instance;
}
