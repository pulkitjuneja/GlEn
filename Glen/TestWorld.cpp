#pragma once 

#ifndef TEST_WORLD
#define TEST_WORLD

#include "Engine.h"
#include "Globals.h"
#include <windows.h>

class TestWorld : public Engine {

public:
	TestWorld() {};
	bool init() {
		return true;
	}

	void update() {
	}
};

#endif // !TEST_WORLD