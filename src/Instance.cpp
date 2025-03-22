#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <map>

#include "Instance.h"
#include "macros.h"
#include "VkUtils.h"

namespace dm
{
    void DMInstance::init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_Window = glfwCreateWindow(1920, 1080, "Drmix", nullptr, nullptr);

        VkApplicationInfo appInfo
        {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "Drmix",
            .applicationVersion = VK_MAKE_VERSION(0, 0, 0),
            .pEngineName = "Drmix Engine",
            .engineVersion = VK_MAKE_VERSION(0, 0, 0),
            .apiVersion = VK_API_VERSION_1_0
        };

        VkInstanceCreateInfo instanceInfo
        {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = 0,
            .ppEnabledExtensionNames = nullptr
        };

        // Extension part
        std::vector<const char*> requiredExtensions(0);

        uint32_t glfwExtensions = 0;
        const char** glfwExtensionsName = nullptr;

        glfwExtensionsName = glfwGetRequiredInstanceExtensions(&glfwExtensions);

        for (uint32_t i = 0; i < glfwExtensions; ++i)
            requiredExtensions.emplace_back(glfwExtensionsName[i]);

        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        requiredExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        
        // Checking for extension support
        uint32_t extCnt = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extCnt, nullptr);
        std::vector<VkExtensionProperties> ext(extCnt);
        vkEnumerateInstanceExtensionProperties(nullptr, &extCnt, ext.data());

        std::map<const std::string, bool> extList;
        std::vector<const char*> requiredExtensionsCopy(requiredExtensions);
        for (VkExtensionProperties extProperty: ext)
        {
            bool isFinded = false;
            uint32_t idx = 0;
            for (uint32_t i = requiredExtensionsCopy.size(); i > 0; --i)
            {
                isFinded = (strcmp(requiredExtensionsCopy[i - 1], extProperty.extensionName) == 0);
                if (isFinded)
                {
                    idx = i - 1;
                    break;
                }
            }

            if (isFinded)
            {
                extList.emplace(extProperty.extensionName, true);
                requiredExtensionsCopy.erase(requiredExtensionsCopy.begin() + idx);
            }
            else
                extList.emplace(extProperty.extensionName, false);
        }

        for (const char* extName : requiredExtensionsCopy)
            spdlog::error("Unfind Extension: {0}", extName);

        spdlog::info("All Extensions:");
        uint32_t index = 0;
        for (std::pair<const std::string, bool> property: extList)
        {
            spdlog::info("[{0:<2}][{1}] {2}", index, property.second ? "X" : " ", property.first);
            ++index;
        }

        instanceInfo.enabledExtensionCount = requiredExtensions.size();
        instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();

        // Layer part
        std::vector<const char*> requiredLayer(0);
        requiredLayer.emplace_back("VK_LAYER_KHRONOS_validation");

        // Checking for layer support
        uint32_t layerCnt = 0;
        vkEnumerateInstanceLayerProperties(&layerCnt, nullptr);
        std::vector<VkLayerProperties> layer(layerCnt);
        vkEnumerateInstanceLayerProperties(&layerCnt, layer.data());

        std::map<const std::string, bool> layerList;
        std::vector<const char*> requiredLayerCopy(requiredLayer);
        for (VkLayerProperties layerProperty: layer)
        {
            bool isFinded = false;
            uint32_t idx = 0;
            for (uint32_t i = requiredLayerCopy.size(); i > 0; --i)
            {
                isFinded = (strcmp(requiredLayerCopy[i - 1], layerProperty.layerName) == 0);
                if (isFinded)
                {
                    idx = i - 1;
                    break;
                }
            }

            if (isFinded)
            {
                layerList.emplace(layerProperty.layerName, true);
                requiredLayerCopy.erase(requiredLayerCopy.begin() + idx);
            }
            else
                layerList.emplace(layerProperty.layerName, false);
        }

        for (const char* layerName : requiredLayerCopy)
            spdlog::error("Unfind Layer: {0}", layerName);

        spdlog::info("AllLayer:");
        index = 0;
        for (std::pair<const std::string, bool> property: layerList)
        {
            spdlog::info("[{0:<2}][{1}] {2}", index, property.second ? "X" : " ", property.first);
            ++index;
        }

        instanceInfo.enabledLayerCount = requiredLayer.size();
        instanceInfo.ppEnabledLayerNames = requiredLayer.data();

        VkDebugUtilsMessengerCreateInfoEXT debugInfo
        {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = nullptr,
            .flags = 0,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = debugCallback,
            .pUserData = nullptr
        };

        instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugInfo;
        
        VkCall(vkCreateInstance(&instanceInfo, nullptr, &m_Instance), "failed create instance");
        
        // VkCall(CreateDebugUtilsMessengerEXT(m_Instance, &debugInfo, nullptr, &m_DebugMessenger), "failed to set up debug messenger!");
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
        vkDestroyInstance(m_Instance, nullptr);
        glfwDestroyWindow(m_Window);

        glfwTerminate();
    }
}