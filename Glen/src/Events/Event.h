#pragma once 

#ifndef Event

#include "../Globals.h"

// TOD: decouple this class from SFML events
// Event class to encapsulate an sfml event;
class Event {
public:
	using EventFunction = std::function<bool(sf::Event&)>;
	sf::Event event;
	sf::Event::EventType type;
	bool isHandled;

	Event(sf::Event event);
	bool Dispatch(sf::Event::EventType type, EventFunction func);
};

#endif // !Event
