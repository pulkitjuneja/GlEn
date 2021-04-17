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

    /*renderer = new ForwardRenderer();*/
    defferedRenderer = new DefferedRenderer();
    defferedRenderer->setScene(scene);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    Timer timer;
    while (isEngineRunning) {
        deltaTime = timer.restart();

        // update step
        update(deltaTime.getAsSeconds());

        // render step
        defferedRenderer->render();
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
    ResourceManager::getInstance()->loadShader("Assets/Shaders/DefferedGeometryPass.vert", "Assets/Shaders/DefferedGeometryPass.frag", "defferedGeometryPass");
    ResourceManager::getInstance()->loadShader("Assets/Shaders/DefferedDirectionalLight.vert", "Assets/Shaders/DefferedDirectionalLight.frag", "defferedDirectionalLightPass");
    ResourceManager::getInstance()->loadShader("Assets/Shaders/DefferedPointLight.vert", "Assets/Shaders/DefferedPointLight.frag", "defferedPointLightPass");
    ResourceManager::getInstance()->loadShader("Assets/Shaders/DefferedDirectionalLight.vert", "Assets/Shaders/HDRToneMapping.frag", "basicToneMapping");
    ResourceManager::getInstance()->loadShader("Assets/Shaders/DefferedDirectionalLight.vert", "Assets/Shaders/SSRPass.frag", "ssrPass");
}

bool Engine::setupWindow() {
 
    window = Window::createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Glen Editor");
    window->SetVsync(false);
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
    PhysicsContext::get()->update();
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
