#ifndef ENGINE_H
#define ENGINE_H

#include "Globals.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "ForwardRenderer.h"
#include "Core.h"

    class GLN_API Engine {

    protected:
        sf::Window* window;
        Scene* scene;
        bool isEngineRunning = true;
        bool setupSFML();
        ForwardRenderer* renderer;

    public:
        static sf::Time deltaTime;
        void loadDefaultShaders();
        static sf::Time timeSinceStart;
        virtual bool init() = 0;
        void update(float deltaTime);
        void start();
    };


#endif //ENGINE_H
