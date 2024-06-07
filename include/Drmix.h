#pragma once

#include <cstdint>
#include <optional>
#include <vulkan/vulkan_core.h>
#include <toml++/toml.hpp>

struct GLFWwindow;

const uint32_t WIDTH = 1280;
const uint32_t HEIGHT = 720;

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    
    operator bool()
    {
        return
        graphicsFamily.has_value() &&
        presentFamily.has_value();
    }
};

struct Queue
{
    VkQueue graphicsQueue;
    VkQueue presetQueue;
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class DrmixApplication 
{
public:
    void run();
private:
    GLFWwindow* window;

    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice;
    VkSurfaceKHR surface;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    toml::table setting;

    QueueFamilyIndices queueFamilyIndices;
    Queue queue;
    SwapChainSupportDetails details;

    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    void createInstance();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();

    void querySwapChainSupport();
};