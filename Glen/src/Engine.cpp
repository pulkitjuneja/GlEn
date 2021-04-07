#include "Engine.h"

sf::Time Engine::deltaTime;
sf::Time Engine::timeSinceStart;

void Engine::start() {

    // engine specific initializations
    if (!setupWindow()) {
        isEngineRunning = false;
    }

    loadDefaultShaders();
    scene = new Scene();

    if (!init()) {
        isEngineRunning = false;
    }

    glEnable(GL_DEPTH_TEST);

    renderer = new ForwardRenderer();
    
    sf::Clock clock;
    while (isEngineRunning) {
        window->processEvents();
        deltaTime = clock.restart();
        timeSinceStart += deltaTime;
        update(deltaTime.asSeconds());
        renderer->render(scene);
        window->Display();
    }

    // todo: perform shutdown steps
    window->shutdown();
}

void Engine::loadDefaultShaders()
{
    ResourceManager::getInstance()->loadShader("Assets/Shaders/DepthMap.vert", "Assets/Shaders/DepthMap.frag", "depthMap");
}

bool Engine::setupWindow() {
    window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, "Glen Engine");
    glewExperimental = GL_TRUE;
    window->setEventCallback(std::bind(&Engine::onWindowEvent,this, std::placeholders::_1));

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    return true;
}

void Engine::update(float deltaTime) {
    scene->update(deltaTime);
}

void Engine::onWindowEvent(sf::Event& sfEvent)
{
    Event event(sfEvent);
    event.Dispatch(sf::Event::EventType::Closed, [=](sf::Event& event)->bool {
        cout << "closed";
        isEngineRunning = false;
        return true;
    });
}
