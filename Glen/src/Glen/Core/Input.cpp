#include "Input.h"


bool InputStatus::isKeyPressed(Keys key) {
	GLFWwindow* window = EngineContext::get()->window->getNativeWindow();
	int state = glfwGetKey(window, static_cast<int>(key));
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool InputStatus::isMouseButtonPressed(MouseButtons button) {
	GLFWwindow* window = EngineContext::get()->window->getNativeWindow();
	int state = glfwGetMouseButton(window, static_cast<int>(button));
	return state == GLFW_PRESS;
}

glm::vec2 InputStatus::getMousePosition() {
	GLFWwindow* window = EngineContext::get()->window->getNativeWindow();
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return glm::vec2(xpos, ypos);
}
