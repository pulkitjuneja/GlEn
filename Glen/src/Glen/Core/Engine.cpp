#include "Engine.h"

Time Engine::deltaTime;

void Engine::start() {

    EngineContext::get()->sceneAllocator = new StackAllocator(1024 * 1024 * 800); // 800 mb

    // engine specific initializations
    if (!setupWindow()) {
        isEngineRunning = false;
    }

    scene = Mem::Allocate<SceneManager>();

    EngineContext::get()->resourceManager = Mem::Allocate<ResourceManager>();
    EngineContext::get()->window = window;
    EngineContext::get()->inputStatus = Mem::Allocate<InputStatus>();
    EngineContext::get()->sceneManager = scene;
    EngineContext::get()->physicsContext = Mem::Allocate<PhysicsContext>();

    loadDefaultShaders();
    EngineContext::get()->resourceManager->loadPrimitives();


    // register core systems
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
        EngineContext::get()->stats.lastFrameTime = deltaTime.getAsMilliSeconds();
        systemManager.update(deltaTime.getAsSeconds());
        window->Display();
        EngineContext::get()->stats.frameCount++;
    }
}

Engine::~Engine()
{
    Logger::logInfo("Shutting down Glen");

    systemManager.shutdown();

    EngineContext::get()->resourceManager->Release();
    scene->Release();
    window->shutdown();
    glfwTerminate();

    EngineContext::get()->sceneAllocator->Release();

    delete EngineContext::get();

}

void Engine::loadDefaultShaders()
{
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/basic.vert", "Assets/Shaders/basic.frag", "depthPass");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DepthMap.vert", "Assets/Shaders/DepthMap.frag", "shadowMap");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedGeometryPass.vert", "Assets/Shaders/DefferedGeometryPass.frag", "defferedGeometryPass");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedDirectionalLight.vert", "Assets/Shaders/DefferedDirectionalLight.frag", "defferedDirectionalLightPass");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedPointLight.vert", "Assets/Shaders/DefferedPointLight.frag", "defferedPointLightPass");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedDirectionalLight.vert", "Assets/Shaders/HDRToneMapping.frag", "basicToneMapping");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedDirectionalLight.vert", "Assets/Shaders/SSRPass.frag", "ssrPass");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/EquirectangularToCubeMap.vert", "Assets/Shaders/EquirectangularToCubeMap.frag", "EqToCm");
    EngineContext::get()->resourceManager->loadComputeShader("Assets/Shaders/LightCulling.comp", "LightCullingCompute");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedDirectionalLight.vert", "Assets/Shaders/LightCullingDebug.frag", "lightDebugShader");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedDirectionalLight.vert", "Assets/Shaders/TextureDebug.frag", "TextureDebugShader");
    EngineContext::get()->resourceManager->loadShader("Assets/Shaders/DefferedDirectionalLight.vert", "Assets/Shaders/TAA.frag", "TAA");
}

bool Engine::setupWindow() {
 
    window = Mem::Allocate<Window>(SCREEN_WIDTH, SCREEN_HEIGHT, "Glen Editor");
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
