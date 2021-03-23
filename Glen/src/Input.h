# pragma once

#ifndef INPUT_H
#define INPUT_H

#include "Globals.h"

class GLN_API Input {
	static Input* instance;
public:
	bool isKeyPressed(sf::Keyboard::Key key);
	bool isMouseButtonPressed(sf::Mouse::Button button);
	glm::vec2 getMousePosition();
	static Input* getInstance();
};
#endif // !INPUT_H
