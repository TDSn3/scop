#include <header.hpp>

void HelloTriangleApplication::printValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    cout << COLOR_DIM << availableLayers.size() << COLOR_RESET << " available layers:\n";

    for (const auto &layerProperties : availableLayers)
        cout << "\t" << layerProperties.layerName << "\n";

    cout << "\n";

    cout << COLOR_DIM << validationLayers.size() << COLOR_RESET << " validation layers required:\n";

    for (const char *layerName : validationLayers) {
        cout << "\t" << layerName;

        bool layerFound = false;

        for (const auto &layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                cout << COLOR_GREEN << " available\n" << COLOR_RESET;
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
            cout << COLOR_RED << " unavailable\n" << COLOR_RESET;
    }

    cout << "\n";    
}

void HelloTriangleApplication::printAvailableVulkanExtension() {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    cout << COLOR_DIM << extensions.size() << COLOR_RESET << " available Vulkan extensions:\n";

    for (const auto &extension : extensions)
        cout << "\t" << extension.extensionName << "\n";

    cout << "\n";
}

void HelloTriangleApplication::printAvailableGlfwExtension(vector<const char *> &glfwExtensions) {
    cout << COLOR_DIM << glfwExtensions.size() << COLOR_RESET << " available GLFW extensions:\n";

    for (size_t i = 0; i < glfwExtensions.size(); i++)
        cout << "\t" << glfwExtensions[i] << "\n";

    cout << "\n";
}

void HelloTriangleApplication::printQueueFamilies(VkPhysicalDevice device) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    cout << "\t" << COLOR_DIM << queueFamilyCount << COLOR_RESET << " available queue families:\n";

    int i = 0;

    for (const auto &queueFamiliesIterator : queueFamilies) {
        cout << "\t\tindex: " << i << "\n";
        cout << "\t\t\tqueue Count: " << queueFamiliesIterator.queueCount << "\n";
        cout << "\t\t\tsupported Operations:\n";
        if (queueFamiliesIterator.queueFlags & VK_QUEUE_GRAPHICS_BIT) cout << "\t\t\t\tgraphics\n";
        if (queueFamiliesIterator.queueFlags & VK_QUEUE_COMPUTE_BIT) cout << "\t\t\t\tcompute\n";
        if (queueFamiliesIterator.queueFlags & VK_QUEUE_TRANSFER_BIT) cout << "\t\t\t\ttransfer\n";
        if (queueFamiliesIterator.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) cout << "\t\t\t\tsparse binding\n";
        std::cout << "\n";

        i++;
    }    
}

void HelloTriangleApplication::printSupportedPhysicalDeviceExtensions() {
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extensionCount, nullptr);

    vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extensionCount, extensions.data());

    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(_physicalDevice, &physicalDeviceProperties);

    cout << COLOR_DIM << extensions.size() << COLOR_RESET << " available physical device extensions for " << physicalDeviceProperties.deviceName << " :\n";

    for (const auto& extension : extensions)
        cout << "\t" << extension.extensionName << "\n";

    cout << "\n";
}
