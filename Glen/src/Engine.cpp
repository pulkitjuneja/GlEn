#include "Engine.h"

sf::Time Engine::deltaTime;
sf::Time Engine::timeSinceStart;

void Engine::start() {

    // engine specific initializations
    if (!setupSFML()) {
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
        sf::Event windowEvent;

        while (window->pollEvent(windowEvent))
        {
            switch (windowEvent.type)
            {
            case sf::Event::Closed:
                isEngineRunning = false;
                break;
            }
        }
        deltaTime = clock.restart();
        timeSinceStart += deltaTime;
        update(deltaTime.asSeconds());
        renderer->render(scene);
        window->display();
    }

    // todo: perform shutdown steps
    window->close();
}

void Engine::loadDefaultShaders()
{
    ResourceManager::getInstance()->loadShader("Assets/Shaders/DepthMap.vert", "Assets/Shaders/DepthMap.frag", "depthMap");
}

bool Engine::setupSFML() {

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    settings.attributeFlags = sf::ContextSettings::Core;

    window = new sf::Window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 64), "OpenGL SFML", sf::Style::Titlebar | sf::Style::Close, settings);
    window->setVerticalSyncEnabled(true);
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    std::string versionString = std::string((const char*)glGetString(GL_VERSION));
    std::cout << versionString.c_str();

    // Load Default Shaders

    return true;
}

void Engine::update(float deltaTime) {
    scene->update(deltaTime);
}
