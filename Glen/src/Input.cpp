#include "Input.h"

Input* Input::instance;


bool Input::isKeyPressed(sf::Keyboard::Key key) {
	return sf::Keyboard::isKeyPressed(key);
}

bool Input::isMouseButtonPressed(sf::Mouse::Button button) {
	return sf::Mouse::isButtonPressed(button);
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