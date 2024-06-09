#pragma once

#include <cstdint>
#include <optional>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <toml++/toml.hpp>

struct GLFWwindow;

const uint32_t WIDTH = 1280;
const uint32_t HEIGHT = 720;

const int MAX_FRAMES_IN_FLIGHT = 2;

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
    VkQueue presentQueue;
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
    std::vector<VkImageView> swapChainImageViews;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffer;
    std::vector<VkSemaphore> imageAvailableSemaphore;
    std::vector<VkSemaphore> renderFinishedSemaphore;
    std::vector<VkFence> inFlightFence;

    toml::table setting;

    QueueFamilyIndices queueFamilyIndices;
    Queue queue;
    SwapChainSupportDetails details;
    uint32_t currentFrame = 0;
    
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    void createInstance();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffer();
    void createSyncObjects();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void querySwapChainSupport();

    void drawFrame();
};