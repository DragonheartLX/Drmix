#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Instance.h"

namespace dm
{
    void DMInstance::init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_Window = glfwCreateWindow(1920, 1080, "Drmix", nullptr, nullptr);
    }

    void DMInstance::mainLoop()
    {
        while (!glfwWindowShouldClose(m_Window))
        {
            glfwPollEvents();
        }
    }

    void DMInstance::cleanup()
    {
        glfwDestroyWindow(m_Window);

        glfwTerminate();
    }
}