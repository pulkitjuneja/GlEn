#include "Window.h"


Window::Window(int width, int height, char* title)
{
    data = WindowData{ title, width, height, true };

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to init GLFW\n");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    printf("GLFW version: %s\n", glfwGetVersionString());

    window = glfwCreateWindow(data.width, data.height, data.title, NULL, NULL);

    if (window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(window, &data);

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
        data->width = width;
        data->height = height;
        glViewport(0, 0, width, height);

        printf("Resized %d, %d\n", data->width, data->height);
        });

    glfwMakeContextCurrent(window);
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    SetVsync(data.vsync);
}

void Window::SetVsync(bool vSync)
{
    data.vsync = vSync;
    glfwSwapInterval(vSync ? 1 : 0);
}

void Window::hookEvents()
{
    glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowClosedEvent event;
            data.eventCallback(event);
        });
}

void Window::setEventCallback(const EventCallback& eventCallback)
{
    this->data.eventCallback = eventCallback;
    glfwSetWindowUserPointer(window, &data);
}

void Window::Display()
{
    glfwPollEvents();
    glfwSwapBuffers(window);
}

void Window::shutdown()
{
    glfwDestroyWindow(window);
}

Window::WindowData& Window::getWindowData()
{
    return data;
}

GLFWwindow* Window::getNativeWindow()
{
    return window;
}
