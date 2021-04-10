#pragma once 

#ifndef Event

#include "Glen/Globals.h"

enum class EventType {
	WindowClose, WindowResize
};

class Event {
public:
	template<typename T>
	using EventFunction = std::function<bool(T&)>;

	bool isHandled;
	EventType eventType;
	std::string name;

	Event(EventType eventType, std::string name);
	template<typename T>	
	bool Dispatch(EventFunction<T> func) {
		if (dynamic_cast<T*>(this)) {
			isHandled = func(*(T*)this);
			return true;
		}
		return false;
	}
	EventType getEventType() { return eventType; }
	virtual ~Event() = default;
};

class WindowClosedEvent : public Event {
public:
	WindowClosedEvent() : Event(EventType::WindowClose, "WindowClosed") {}
	static EventType GetType() { return EventType::WindowClose; }
};

class WindowResizedEvent : public Event {
public:
	WindowResizedEvent() : Event(EventType::WindowResize, "WindowResized") {}
};

#endif // !Event
