#pragma once

#include <cstdint>
#include <vulkan/vulkan_core.h>

struct GLFWwindow;

const uint32_t WIDTH = 1280;
const uint32_t HEIGHT = 720;

class DrmixApplication 
{
public:
    void run();
private:
    GLFWwindow* window;

    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    void createInstance();
    void pickPhysicalDevice();
};