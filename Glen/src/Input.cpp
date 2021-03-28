#include "Input.h"

Input* Input::instance;


bool Input::isKeyPressed(Keys key) {
	return sf::Keyboard::isKeyPressed((sf::Keyboard::Key)key);
}

bool Input::isMouseButtonPressed(MouseButtons button) {
	return sf::Mouse::isButtonPressed((sf::Mouse::Button)button);
}

glm::vec2 Input::getMousePosition() {
	sf::Vector2i position = sf::Mouse::getPosition();
	return glm::vec2(position.x, position.y);
}

Input* Input::getInstance() {
	if (instance == nullptr)
	{
		instance = new Input();
	}
	return instance;
}