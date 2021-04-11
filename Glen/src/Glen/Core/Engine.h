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
#include "Glen/Editor/EditorGui.h"

    class GLN_API Engine {

    protected:
        Window* window;
        Scene* scene;
        bool isEngineRunning = true;
        bool setupWindow();
        ForwardRenderer* renderer;
        DefferedRenderer* defferedRenderer;
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
