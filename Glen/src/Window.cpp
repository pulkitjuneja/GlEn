#include "Window.h"

Window::Window(int width, int height, char* title)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    this->width = width;
    this->height = height;

    settings.attributeFlags = sf::ContextSettings::Core;
    window = new sf::Window(sf::VideoMode(width, height, 64), title,
        sf::Style::Titlebar | sf::Style::Close, settings);
    window->setVerticalSyncEnabled(true);
}

void Window::setVsync(bool vSync)
{
    window->setVerticalSyncEnabled(vSync);
}

void Window::processEvents()
{
    sf::Event event;
    while (window->pollEvent(event)) {
        eventCallback(event);
    }   
}

void Window::setEventCallback(const EventCallback& eventCallback)
{
    this->eventCallback = eventCallback;
}
