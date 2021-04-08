#ifndef ENGINE_H
#define ENGINE_H

#include "Globals.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "ForwardRenderer.h"
#include "Events/Event.h"
#include "Window.h"
#include "Timer.h"
#include "Editor/EditorGui.h"

    class GLN_API Engine {

    protected:
        Window* window;
        Scene* scene;
        bool isEngineRunning = true;
        bool setupWindow();
        ForwardRenderer* renderer;
        EditorGui* Editor;

    public:
        static Time deltaTime;
        void loadDefaultShaders();
        virtual bool init() = 0;
        void update(float deltaTime);
        void onWindowEvent(Event& event);
        void start();
    };


#endif //ENGINE_H
