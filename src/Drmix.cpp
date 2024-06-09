#include "Drmix.h"
#include "Logger.h"
#include "Utility.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
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

static std::vector<char> readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
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
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandPool();
    createCommandBuffer();
    createSyncObjects();
}

void DrmixApplication::mainLoop() 
{
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(logicalDevice);
}

void DrmixApplication::cleanup() 
{
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        vkDestroySemaphore(logicalDevice, imageAvailableSemaphore[i], nullptr);
        vkDestroySemaphore(logicalDevice, renderFinishedSemaphore[i], nullptr);
        vkDestroyFence(logicalDevice, inFlightFence[i], nullptr);
    }

    vkDestroyCommandPool(logicalDevice, commandPool, nullptr);

    for (VkFramebuffer framebuffer : swapChainFramebuffers)
        vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);

    vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
    vkDestroyRenderPass(logicalDevice, renderPass, nullptr);

    for (VkImageView view: swapChainImageViews)
        vkDestroyImageView(logicalDevice, view, nullptr);

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
    vkGetDeviceQueue(logicalDevice, queueFamilyIndices.graphicsFamily.value(), 0, &queue.presentQueue);
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

void DrmixApplication::createImageViews()
{
    swapChainImageViews.resize(swapChainImages.size());
    
    for (int i = 0; i < swapChainImageViews.size(); ++i)
    {
        VkImageViewCreateInfo createInfo = {};

        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];

        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VkCall(vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapChainImageViews[i]), "failed to create image views!");
    }
}

void DrmixApplication::createRenderPass()
{
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VkCall(vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass), "failed to create render pass!");
}

void DrmixApplication::createGraphicsPipeline()
{
    std::vector<char> vertShaderCode = readFile("cache/vert.spv");
    std::vector<char> fragShaderCode = readFile("cache/frag.spv");

    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

    VkShaderModuleCreateInfo vInfo = {};
    vInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vInfo.codeSize = vertShaderCode.size();
    vInfo.pCode = reinterpret_cast<const uint32_t*>(vertShaderCode.data());
    VkCall(vkCreateShaderModule(logicalDevice, &vInfo, nullptr, &vertShaderModule), "failed to create vertex shader module!");

    VkShaderModuleCreateInfo fInfo = {};
    fInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fInfo.codeSize = fragShaderCode.size();
    fInfo.pCode = reinterpret_cast<const uint32_t*>(fragShaderCode.data());
    VkCall(vkCreateShaderModule(logicalDevice, &fInfo, nullptr, &fragShaderModule), "failed to create fragment shader module!");

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates = 
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    VkCall(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout), "failed to create pipeline layout!")

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;

    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;

    pipelineInfo.layout = pipelineLayout;

    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    VkCall(vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline), "failed to create graphics pipeline!");

    vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
}

void DrmixApplication::createFramebuffers()
{
    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        VkImageView attachments[] =
        {
            swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        VkCall(vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]), "failed to create framebuffer!");
    }
}

void DrmixApplication::createCommandPool()
{
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    VkCall(vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool), "failed to create command pool!");
}

void DrmixApplication::createCommandBuffer()
{
    commandBuffer.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = commandBuffer.size();

    VkCall(vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffer.data()), "failed to allocate command buffers!");
}

void DrmixApplication::createSyncObjects()
{
    imageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFence.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        VkCall
        (
            vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore[i]) != VK_SUCCESS ||
            vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore[i]) != VK_SUCCESS ||
            vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFence[i]),
            "failed to create synchronization objects for a frame!"
        );
    }
}

void DrmixApplication::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkCall(vkBeginCommandBuffer(commandBuffer, &beginInfo), "failed to begin recording command buffer!");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChainExtent.width;
    viewport.height = (float) swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);            

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    VkCall(vkEndCommandBuffer(commandBuffer), "failed to record command buffer!");
}

void DrmixApplication::drawFrame()
{
    vkWaitForFences(logicalDevice, 1, &inFlightFence[currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(logicalDevice, 1, &inFlightFence[currentFrame]);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(logicalDevice, swapChain, UINT64_MAX, imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(commandBuffer[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    recordCommandBuffer(commandBuffer[currentFrame], imageIndex);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer[currentFrame];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    VkCall(vkQueueSubmit(queue.graphicsQueue, 1, &submitInfo, inFlightFence[currentFrame]), "failed to submit draw command buffer!");

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(queue.presentQueue, &presentInfo);
    
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}