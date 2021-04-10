#include "Input.h"

Input* Input::instance;


bool Input::isKeyPressed(Keys key) {
	GLFWwindow* window = Window::get()->getNativeWindow();
	int state = glfwGetKey(window, static_cast<int>(key));
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::isMouseButtonPressed(MouseButtons button) {
	GLFWwindow* window = Window::get()->getNativeWindow();
	int state = glfwGetMouseButton(window, static_cast<int>(button));
	return state == GLFW_PRESS;
}

glm::vec2 Input::getMousePosition() {
	GLFWwindow* window = Window::get()->getNativeWindow();
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return glm::vec2(xpos, ypos);
}

Input* Input::getInstance() {
	if (instance == nullptr)
	{
		instance = new Input();
	}
	return instance;
}