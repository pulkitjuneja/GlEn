#include "Engine.h"

Time Engine::deltaTime;

void Engine::start() {

    // engine specific initializations
    if (!setupWindow()) {
        isEngineRunning = false;
    }

    scene = new SceneManager();

    EngineContext::get()->resourceManager = new ResourceManager();
    EngineContext::get()->window = window;
    EngineContext::get()->inputStatus = new InputStatus();
    EngineContext::get()->sceneManager = scene;
    EngineContext::get()->physicsContext = new PhysicsContext();

    loadDefaultShaders();

    systemManager.registerSystem<ScriptingSystem>();
    systemManager.registerSystem<PhysicsSystem>();
    systemManager.registerSystem<DefferedRenderer>();
    systemManager.registerSystem<DebugDraw>();
    systemManager.registerSystem<GuiSystem>();

    systemManager.initialize();


    if (!init()) {
        isEngineRunning = false;
    }
    
    Timer timer;
    while (isEngineRunning) {
        deltaTime = timer.restart();

        // update step
        systemManager.update(deltaTime.getAsSeconds());

        window->Display();
    }

    // todo: perform shutdown steps
    window->shutdown();
    glfwTerminate();
}

void Engine::loadDefaultShaders()
{
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DepthMap.vert", "Assets/Shaders/DepthMap.frag", "depthMap");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedGeometryPass.vert", "Assets/Shaders/DefferedGeometryPass.frag", "defferedGeometryPass");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedDirectionalLight.vert", "Assets/Shaders/DefferedDirectionalLight.frag", "defferedDirectionalLightPass");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedPointLight.vert", "Assets/Shaders/DefferedPointLight.frag", "defferedPointLightPass");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedDirectionalLight.vert", "Assets/Shaders/HDRToneMapping.frag", "basicToneMapping");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedDirectionalLight.vert", "Assets/Shaders/SSRPass.frag", "ssrPass");
}

bool Engine::setupWindow() {
 
    window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, "Glen Editor");
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

void Engine::onWindowEvent(Event& event)
{
    event.Dispatch<WindowClosedEvent>([=](Event& event)->bool {
        std::cout << "closed";
        isEngineRunning = false;
        return true;
    });
}
