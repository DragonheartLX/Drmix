#include "Drmix.h"
#include "Logger.h"

#include <vector>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

void DrmixApplication::run() 
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void DrmixApplication::initWindow() 
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Drmix", nullptr, nullptr);
}

void DrmixApplication::initVulkan() 
{
    uint32_t count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

    std::vector<VkExtensionProperties> properties(count);
    vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data());

    Logger::debug("Support extensions:");
    for (VkExtensionProperties property : properties) 
    {
        Logger::debug("\t{0}", property.extensionName);
    }
}

void DrmixApplication::mainLoop() 
{
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();
    }
}

void DrmixApplication::cleanup() 
{
    glfwDestroyWindow(window);
    glfwTerminate();
}