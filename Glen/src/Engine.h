#ifndef ENGINE_H
#define ENGINE_H

#include "Globals.h"
#include "ResourceManager.h"

class Engine {

protected:
    sf::Window* window;
    bool isEngineRunning = true;
    bool setupSFML();

public:
    static sf::Time deltaTime;
    static sf::Time timeSinceStart;
    void loadDefaultShaders();
    virtual bool init() = 0;
    virtual void update() = 0;
    void start();
};


#endif //ENGINE_H
