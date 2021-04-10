#include "Event.h"

Event::Event(EventType eventType, std::string name)
{
	this->eventType = eventType;
	this->name = name;
	this->isHandled = false;
}