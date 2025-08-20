#include <header.hpp>

void HelloTriangleApplication::createLogicalDevice() {
    VkDeviceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    QueueFamilyIndices QFindices = findQueueFamilies(_physicalDevice, VK_QUEUE_GRAPHICS_BIT);
    printQueueFamilyIndices(QFindices);
    set<uint32_t> uniqueQueueFamilies = { QFindices.graphicsFamily.value(), QFindices.presentFamily.value() };

    float queuePriority = 1.0f;

    for (auto queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};

        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
        cout << COLOR_GREEN << "queue created with queue family index " << COLOR_BOLD << queueFamily << COLOR_RESET << "\n\n";
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    printSupportedPhysicalDeviceExtensions();

    vector<const char *> logicalDeviceExtensions = getRequiredLogicalDeviceExtensions();
    
    createInfo.enabledExtensionCount = static_cast<uint32_t>(logicalDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = logicalDeviceExtensions.data();

    VkPhysicalDeviceFeatures deviceFeatures{};

    createInfo.pEnabledFeatures = &deviceFeatures;

    VkResult result = vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device);

    if (result != VK_SUCCESS)
        throw runtime_error("failed to create logical device!");

    vkGetDeviceQueue(_device, QFindices.graphicsFamily.value(), 0, &_graphicsQueue);
    vkGetDeviceQueue(_device, QFindices.presentFamily.value(), 0, &_presentQueue);

    cout << COLOR_GREEN << "logical device created" << COLOR_RESET << "\n\n";
}

vector<const char *> HelloTriangleApplication::getRequiredLogicalDeviceExtensions() {
    vector<const char *> logicalDeviceExtensions = physicalDeviceExtensions;

    #ifdef __APPLE__
        logicalDeviceExtensions.push_back("VK_KHR_portability_subset");
    #endif

    return logicalDeviceExtensions;
}
