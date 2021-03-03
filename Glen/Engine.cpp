#include "Engine.h"

sf::Time Engine::deltaTime;
sf::Time Engine::timeSinceStart;


void Engine::loadDefaultShaders()
{
}

void Engine::start() {

    // engine specific initializations
    if (!setupSFML()) {
        isEngineRunning = false;
    }


    if (!init()) {
        isEngineRunning = false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(MessageCallback, 0);

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
        update();

        window->display();
    }

    // todo: perform shutdown steps
    window->close();
}

bool Engine::setupSFML() {

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    settings.attributeFlags = sf::ContextSettings::Core;

    window = new sf::Window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 64), "OpenGL SFML", sf::Style::Titlebar | sf::Style::Close, settings);
    window->setVerticalSyncEnabled(false);
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    std::string versionString = std::string((const char*)glGetString(GL_VERSION));
    std::cout << versionString.c_str();

    // Load Default Shaders

    loadDefaultShaders();

    return true;
}
