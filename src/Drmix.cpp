#include "Drmix.h"
#include "Logger.h"
#include "Utility.h"

#include <cstdint>
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
    pickPhysicalDevice();
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
    
    Logger::debug("Support extensions:");
    for (VkExtensionProperties property : EXTproperties)
        Logger::debug("\t{0}", property.extensionName);

    vkEnumerateInstanceLayerProperties(&count, nullptr);

    std::vector<VkLayerProperties> LAYproperties(count);
    vkEnumerateInstanceLayerProperties(&count, LAYproperties.data());

    std::vector<const char*> layers(0);

    Logger::debug("Support layers:");
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

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    Logger::debug("GLFW extensions:");
    for (int i = 0; i < glfwExtensionCount; ++i) 
        Logger::debug("\t{}", glfwExtensions[i]);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = layers.size();
    createInfo.ppEnabledLayerNames = layers.data();
    createInfo.flags = 0;
    createInfo.pNext = nullptr;

    Logger::debug("Enabled layers:");
    for (const char* property : layers) 
        Logger::debug("\t{0}", property);

    Logger::debug("Enabled extensions:");
    for (int i = 0; i < glfwExtensionCount; ++i) 
        Logger::debug("\t{}", glfwExtensions[i]);

    VkCall(vkCreateInstance(&createInfo, nullptr, &instance), "Failed to create instance!");
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

    if (physicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("failed to find a suitable GPU!");
}