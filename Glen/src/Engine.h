#ifndef ENGINE_H
#define ENGINE_H

#include "Globals.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "ForwardRenderer.h"

class Engine {

protected:
    sf::Window* window;
    Scene* scene;
    bool isEngineRunning = true;
    bool setupSFML();
    ForwardRenderer* renderer;

public:
    static sf::Time deltaTime;
    static sf::Time timeSinceStart;
    void loadDefaultShaders();
    virtual bool init() = 0;
    virtual void update() = 0;
    void start();
};


#endif //ENGINE_H
