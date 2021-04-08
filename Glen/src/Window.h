#pragma once

#ifndef WINDOW_H

#include "Globals.h"
#include "Events/Event.h"

// Class to abstract out information about the desktop window
class Window {
public:
	using EventCallback = std::function<void(Event&)>;

	struct WindowData
	{
		const char* title;
		int width;
		int height;
		bool vsync;
		EventCallback eventCallback;
	};

	static Window* createWindow(int width, int height, char* title);
	static Window* get() { return currentWindow; }

	void setEventCallback(const EventCallback& eventCallback);
	void SetVsync(bool vsync);
	void hookEvents();
	void Display(); 
	void shutdown();
	GLFWwindow* getNativeWindow();
private:
	Window(int width, int height, char* title);
	WindowData data;
	GLFWwindow* window;

	static Window* currentWindow;

};

#endif // !WINDOW_H
