#include "Event.h"

Event::Event(sf::Event event)
{
	this->event = event;
	this->type = event.type;
	this->isHandled = false;
}

bool Event::Dispatch(sf::Event::EventType eventType, EventFunction func)
{
	if (this->type == eventType) {
		isHandled = func(event);
		return true;
	}
	return false;
}


