#pragma once

#ifndef WINDOW_H

#include "Globals.h"

// Class to abstract out information about the desktop window
class Window {
public:
	using EventCallback = std::function<void(sf::Event&)>;
	Window(int width, int height, char* title);
	void setVsync(bool vsync);
	void processEvents();
	void setEventCallback(const EventCallback& eventCallback);
	void Display() { window->display();}
	void shutdown() { window->close(); }
private:
	int width;
	int height;
	EventCallback eventCallback;
	sf::Window* window;

};

#endif // !WINDOW_H
