#include "Drmix.h"
#include "Logger.h"
#include "Utility.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <set>
#include <toml++/impl/parser.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

static const char* physicalDeviceTypeString(VkPhysicalDeviceType type)
	{
		switch (type)
		{
#define STR(r) case VK_PHYSICAL_DEVICE_TYPE_ ##r: return #r
			STR(OTHER);
			STR(INTEGRATED_GPU);
			STR(DISCRETE_GPU);
			STR(VIRTUAL_GPU);
			STR(CPU);
#undef STR
		default: return "UNKNOWN_DEVICE_TYPE";
		}
	}

void DrmixApplication::run() 
{
    setting = toml::parse_file("./res/settings.toml");


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
    createInstance();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    querySwapChainSupport();
    createSwapChain();
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
    vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
    vkDestroyDevice(logicalDevice, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    
    glfwDestroyWindow(window);
    glfwTerminate();
}

void DrmixApplication::createInstance()
{
    uint32_t count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

    std::vector<VkExtensionProperties> EXTproperties(count);
    vkEnumerateInstanceExtensionProperties(nullptr, &count, EXTproperties.data());
    
    std::vector<const char*> extensions(0);

    Logger::debug("Instance support extensions:");
    for (VkExtensionProperties property : EXTproperties)
    {
        Logger::debug("\t{0}", property.extensionName);
    }
        

    vkEnumerateInstanceLayerProperties(&count, nullptr);

    std::vector<VkLayerProperties> LAYproperties(count);
    vkEnumerateInstanceLayerProperties(&count, LAYproperties.data());

    std::vector<const char*> layers(0);

    Logger::debug("Instance support layers:");
    for (VkLayerProperties property : LAYproperties) 
    {
        Logger::debug("\t{0}", property.layerName);

        if (strcmp(property.layerName, "VK_LAYER_KHRONOS_validation") == 0)
        {
            layers.push_back("VK_LAYER_KHRONOS_validation");
        }
    }
    
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Drmix";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    appInfo.pNext = nullptr;

    VkInstanceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        for (uint32_t i = 0; i < glfwExtensionCount; ++i)
        {
            extensions.push_back(glfwExtensions[i]);
        }
    }

    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    createInfo.enabledLayerCount = layers.size();
    createInfo.ppEnabledLayerNames = layers.data();
    createInfo.flags = 0;
    createInfo.pNext = nullptr;

    VkCall(vkCreateInstance(&createInfo, nullptr, &instance), "Failed to create instance!");

    Logger::debug("Enabled instance layers:");
    for (const char* property : layers) 
        Logger::debug("\t{0}", property);

    Logger::debug("Enabled instance extensions:");
    for (const char* property : extensions) 
        Logger::debug("\t{}", property);
}

void DrmixApplication::createSurface()
{
    VkCall(glfwCreateWindowSurface(instance, window, nullptr, &surface), "failed to create window surface!");
}

void DrmixApplication::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
        throw std::runtime_error("failed to find GPUs with Vulkan support!");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
    Logger::debug("Available Devices:");
    for (int i = 0; i < devices.size(); ++i) 
    {
        VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);

        const char* type = physicalDeviceTypeString(deviceProperties.deviceType);
        uint32_t version[3] =
        {
            deviceProperties.apiVersion >> 22,
            deviceProperties.apiVersion >> 12,
            deviceProperties.apiVersion & 0xfff
        };

        Logger::debug("Index: [{0}]", i);
        Logger::debug("\tDevice: {0}", deviceProperties.deviceName);
		Logger::debug("\tType: {0}", type);
		Logger::debug("\tAPI: {0}.{1}.{2}", version[0], version[1], version[2]);
        Logger::debug("");
    }

    // pickup
    
    // use setting
    int32_t deviceIndex = setting["Application"]["GPUIndex"].value_or(0);

    // if use setting, check support

    physicalDevice = devices[deviceIndex];

    if (physicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("failed to find a suitable GPU!");

    Logger::debug("Use GPU: Index[{}]", deviceIndex);

    // find queue
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    for (int i = 0; i < queueFamilies.size(); ++i)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)      
            queueFamilyIndices.graphicsFamily = i;

        VkBool32 isSupport;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &isSupport);

        if (isSupport)
            queueFamilyIndices.presentFamily = i;

        if (queueFamilyIndices) 
            break;
    }

    if (!queueFamilyIndices)
        throw std::runtime_error("GPU not support graphics queue!");
}

void DrmixApplication::createLogicalDevice()
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    
    std::vector<VkExtensionProperties> EXTproperties(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, EXTproperties.data());

    std::vector<const char*> extensions(0);

    Logger::debug("Device support extensions:");
    for (VkExtensionProperties property : EXTproperties)
    {
        Logger::debug("\t{0}", property.extensionName);

        if (strcmp(property.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
            extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    uint32_t layerCount;
    vkEnumerateDeviceLayerProperties(physicalDevice, &layerCount, nullptr);
    
    std::vector<VkLayerProperties> LAYproperties(layerCount);
    vkEnumerateDeviceLayerProperties(physicalDevice, &layerCount, LAYproperties.data());

    std::vector<const char*> layers(0);

    Logger::debug("Device support layers:");
    for (VkLayerProperties property : LAYproperties)
    {
        Logger::debug("\t{0}", property.layerName);

        if (strcmp(property.layerName, "VK_LAYER_KHRONOS_validation") == 0)
            layers.push_back("VK_LAYER_KHRONOS_validation");
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(0);

    std::set<uint32_t> uniqueQueueFamilies =
    {
        queueFamilyIndices.graphicsFamily.value(),
        queueFamilyIndices.presentFamily.value()
    };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        createInfo.queueFamilyIndex = queueFamily;
        createInfo.queueCount = 1;
        createInfo.pQueuePriorities = &queuePriority;
        createInfo.flags = 0;
        createInfo.pNext = nullptr;

        queueCreateInfos.push_back(createInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    
    VkDeviceCreateInfo deviceCreateInfo {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    deviceCreateInfo.enabledExtensionCount = extensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = extensions.data();

    deviceCreateInfo.enabledLayerCount = layers.size();
    deviceCreateInfo.ppEnabledLayerNames = layers.data();
    deviceCreateInfo.pNext = nullptr;

    VkCall(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice), "failed to create logical device!");
    
    Logger::debug("Enabled device layers:");
    for (const char* property : layers) 
        Logger::debug("\t{0}", property);

    Logger::debug("Enabled device extensions:");
    for (const char* property : extensions) 
        Logger::debug("\t{}", property);
    
    vkGetDeviceQueue(logicalDevice, queueFamilyIndices.graphicsFamily.value(), 0, &queue.graphicsQueue);
    vkGetDeviceQueue(logicalDevice, queueFamilyIndices.graphicsFamily.value(), 0, &queue.presetQueue);
}

void DrmixApplication::createSwapChain()
{
    VkSurfaceFormatKHR surfaceFormat = details.formats[0];
    for (VkSurfaceFormatKHR sf : details.formats)
    {
        if (sf.format == VK_FORMAT_B8G8R8A8_SRGB && sf.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            surfaceFormat = sf;
    }

    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (VkPresentModeKHR pm : details.presentModes)
    {
        if (pm == VK_PRESENT_MODE_MAILBOX_KHR)
            presentMode = pm;
    }

    VkExtent2D extent;
    if (details.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        extent = details.capabilities.currentExtent;
    } 
    else 
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = 
        {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, details.capabilities.minImageExtent.width, details.capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, details.capabilities.minImageExtent.height, details.capabilities.maxImageExtent.height);

        extent =  actualExtent;
    }

    uint32_t imageCount = details.capabilities.minImageCount + 1;
    if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
        imageCount = details.capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t indices[] = 
    {
        queueFamilyIndices.graphicsFamily.value(),
        queueFamilyIndices.presentFamily.value()
    };

    if (indices[0] != indices[1])
    {

        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = indices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;
    createInfo.flags = 0;
    createInfo.pNext = nullptr;

    VkCall(vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain), "failed to create swap chain!");

    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

void DrmixApplication::querySwapChainSupport()
{
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    
    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
    }
    
    uint32_t presetModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presetModeCount, nullptr);

    if (presetModeCount != 0)
    {
        details.presentModes.resize(presetModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presetModeCount, details.presentModes.data());
    }

    // check support
}