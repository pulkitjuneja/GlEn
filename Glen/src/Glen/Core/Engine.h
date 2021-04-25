#ifndef ENGINE_H
#define ENGINE_H

#include "Glen/Globals.h"
#include "Glen/ResourceManagement/ResourceManager.h"
#include "Glen/Scene/Scene.h"
#include "Glen/Rendering/ForwardRenderer.h"
#include "Glen/Rendering/DeffererdRenderer.h"
#include "Glen/Events/Event.h"
#include "Window.h"
#include "Timer.h"
#include "Glen/Editor/GuiSystem.h"
#include "Glen/Physics/PhysicsContext.h"
#include "EngineContext.h"
#include "Input.h"

#include "Glen/Physics/PhysicsSystem.h"
#include "Glen/Scene/ScriptingSystem.h"

    class GLN_API Engine {

    protected:
        Window* window;
        SceneManager* scene;
        bool isEngineRunning = true;
        bool setupWindow();

        SystemManager systemManager;

    public:
        static Time deltaTime;
        void loadDefaultShaders();
        virtual bool init() = 0;
        void onWindowEvent(Event& event);
        void start();
    };


#endif //ENGINE_H
