#ifndef ENGINE_H
#define ENGINE_H

#include "Globals.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "ForwardRenderer.h"
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

    class GLN_API Engine {

    protected:
        Window* window;
        Scene* scene;
        bool isEngineRunning = true;
        bool setupWindow();
        ForwardRenderer* renderer;

    public:
        static sf::Time deltaTime;
        void loadDefaultShaders();
        static sf::Time timeSinceStart;
        virtual bool init() = 0;
        void update(float deltaTime);
        void onWindowEvent(sf::Event& event);
        void start();
    };


#endif //ENGINE_H
