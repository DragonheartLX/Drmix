#pragma once
#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace dm
{
    class DMInstance
    {
    public:
        void init();
        void mainLoop();
        void cleanup();
    private:
        GLFWwindow* m_Window = nullptr;
        VkInstance m_Instance = nullptr;

        VkDebugUtilsMessengerEXT m_DebugMessenger;
    };
}