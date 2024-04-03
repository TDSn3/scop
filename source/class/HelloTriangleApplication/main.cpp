#include <header.hpp>

void HelloTriangleApplication::createVulkanInstance () {
    printValidationLayerSupport();
    
    if (!checkValidationLayerSupport())
        throw runtime_error("validation layers requested, but not available!");

    VkInstanceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;

    #ifdef __APPLE__
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif

    VkApplicationInfo appInfo{};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    createInfo.pApplicationInfo = &appInfo;

    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    vector<const char *> glfwExtensions = getRequiredGlfwExtensions();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
    createInfo.ppEnabledExtensionNames = glfwExtensions.data();

    printAvailableVulkanExtension();
    printAvailableGlfwExtension(glfwExtensions);

    VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);

    if (result != VK_SUCCESS)
        throw runtime_error("failed to create Vulkan instance: error " + to_string(result) + "\n"); // error code : https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkResult.html
}

bool HelloTriangleApplication::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : validationLayers) {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

vector<const char *> HelloTriangleApplication::getRequiredGlfwExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    vector<const char*> glfwExtensionsExtend(glfwExtensions, glfwExtensions + glfwExtensionCount);

    #ifdef __APPLE__
        glfwExtensionsExtend.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        glfwExtensionsExtend.push_back("VK_KHR_get_physical_device_properties2");
    #endif

    glfwExtensionsExtend.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return glfwExtensionsExtend;
}

void HelloTriangleApplication::pickPhysicalDevice() {
    printAvailablePhysicalDevices();
    
    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, nullptr);

    if (physicalDeviceCount == 0)
        throw runtime_error("failed to find GPUs with Vulkan support!");

    vector<VkPhysicalDevice> physicalDevice(physicalDeviceCount);
    vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, physicalDevice.data());

    for (const auto &physicalDeviceIterator : physicalDevice) {
        if (isDeviceSuitable(physicalDeviceIterator)) {
            _physicalDevice = physicalDeviceIterator;
            break;
        }
    }

    if (_physicalDevice == VK_NULL_HANDLE)
        throw runtime_error("failed to find a suitable GPU!");
}

bool HelloTriangleApplication::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(device, VK_QUEUE_GRAPHICS_BIT);

    return indices.isComplete();
}

QueueFamilyIndices HelloTriangleApplication::findQueueFamilies(VkPhysicalDevice device, VkQueueFlagBits flags) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;

    for (const auto &queueFamiliesIterator : queueFamilies) {
        if (queueFamiliesIterator.queueFlags & flags)
            indices.graphicsFamily = i;
        i++;
    }

    return indices;
}

void HelloTriangleApplication::createLogicalDevice() {
    VkDeviceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    VkDeviceQueueCreateInfo queueCreateInfo{};

    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

    QueueFamilyIndices indices = findQueueFamilies(_physicalDevice, VK_QUEUE_GRAPHICS_BIT);
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();

    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;

    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    printSupportedPhysicalDeviceExtensions();

    vector<const char *> logicalDeviceExtensions = getRequiredLogicalDeviceExtensions();
    
    createInfo.enabledExtensionCount = static_cast<uint32_t>(logicalDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = logicalDeviceExtensions.data();

    VkPhysicalDeviceFeatures deviceFeatures{};

    createInfo.pEnabledFeatures = &deviceFeatures;

    VkResult result = vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device);

    if (result != VK_SUCCESS)
        throw runtime_error("failed to create logical device!");
    
    vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
}

vector<const char *> HelloTriangleApplication::getRequiredLogicalDeviceExtensions() {
    vector<const char *> logicalDeviceExtensions;

    #ifdef __APPLE__
        logicalDeviceExtensions.push_back("VK_KHR_portability_subset");
    #endif

    return logicalDeviceExtensions;
}

void HelloTriangleApplication::createSurface() {
    if (glfwCreateWindowSurface(_instance, _window, nullptr, &_surface) != VK_SUCCESS) {
        throw runtime_error("failed to create window surface!");
    }
}
