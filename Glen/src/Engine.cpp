#include "Engine.h"

Time Engine::deltaTime;

void Engine::start() {

    // engine specific initializations
    if (!setupWindow()) {
        isEngineRunning = false;
    }

    loadDefaultShaders();
    scene = new Scene();

    Editor = new EditorGui(scene);

    if (!init()) {
        isEngineRunning = false;
    }

    glEnable(GL_DEPTH_TEST);

    renderer = new ForwardRenderer();
    
    Timer timer;
    while (isEngineRunning) {
        deltaTime = timer.restart();

        // update step
        update(deltaTime.getAsSeconds());

        // render step
        renderer->render(scene);
        Editor->render();

        window->Display();
    }

    // todo: perform shutdown steps
    window->shutdown();
    glfwTerminate();
}

void Engine::loadDefaultShaders()
{
    ResourceManager::getInstance()->loadShader("Assets/Shaders/DepthMap.vert", "Assets/Shaders/DepthMap.frag", "depthMap");
}

bool Engine::setupWindow() {
 
    window = Window::createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Glen Editor");
    window->setEventCallback(std::bind(&Engine::onWindowEvent, this, std::placeholders::_1));
    window->hookEvents();

    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    return true;
}

void Engine::update(float deltaTime) {
    scene->update(deltaTime);
    Editor->update();
}

void Engine::onWindowEvent(Event& event)
{
    event.Dispatch<WindowClosedEvent>([=](Event& event)->bool {
        cout << "closed";
        isEngineRunning = false;
        return true;
    });
}
