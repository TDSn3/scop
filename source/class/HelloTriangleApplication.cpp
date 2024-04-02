#include <header.hpp>

void HelloTriangleApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void HelloTriangleApplication::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::initVulkan() {
    createVulkanInstance();
    setupDebugMessenger();
    pickPhysicalDevice();
    createLogicalDevice();
}

void HelloTriangleApplication::mainLoop() {
    while (!glfwWindowShouldClose(_window)) glfwPollEvents();
}

void HelloTriangleApplication::cleanup() {
    vkDestroyDevice(_device, nullptr);
    DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
    vkDestroyInstance(_instance, nullptr);
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void HelloTriangleApplication::createVulkanInstance () {
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

        if (!layerFound) {
            cout << COLOR_RED << " unavailable\n" << COLOR_RESET;
            return false;
        }
    }

    cout << "\n";

    return true;
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

void HelloTriangleApplication::setupDebugMessenger() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo;

    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS)
        throw runtime_error("failed to set up debug messenger!");
}

void HelloTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

void HelloTriangleApplication::pickPhysicalDevice() {
    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, nullptr);

    if (physicalDeviceCount == 0)
        throw runtime_error("failed to find GPUs with Vulkan support!");

    vector<VkPhysicalDevice> physicalDevice(physicalDeviceCount);
    vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, physicalDevice.data());

    cout << COLOR_DIM << physicalDeviceCount << COLOR_RESET << " available physical devices:\n";

    for (const auto &physicalDeviceIterator : physicalDevice) {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(physicalDeviceIterator, &physicalDeviceProperties);

        cout << "\t" << physicalDeviceProperties.deviceName;

        string deviceTypeName;
        const auto deviceTypeValue = physicalDeviceProperties.deviceType;

        if      (deviceTypeValue == VK_PHYSICAL_DEVICE_TYPE_OTHER)          deviceTypeName = "other";
        else if (deviceTypeValue == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) deviceTypeName = "integrated GPU";
        else if (deviceTypeValue == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)   deviceTypeName = "discrete GPU";
        else if (deviceTypeValue == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)    deviceTypeName = "virtual GPU";
        else if (deviceTypeValue == VK_PHYSICAL_DEVICE_TYPE_CPU)            deviceTypeName = "CPU";
        else                                                                deviceTypeName = "unknown";

        cout << " " << COLOR_BLUE << deviceTypeName << COLOR_RESET;

        if (isDeviceSuitable(physicalDeviceIterator)) {
            _physicalDevice = physicalDeviceIterator;
            cout << COLOR_GREEN << " suitable" << COLOR_RESET << "\n";
            printQueueFamilies(physicalDeviceIterator);
            break;
        }

        cout << "not suitable" << COLOR_RESET << "\n";
        printQueueFamilies(physicalDeviceIterator);
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

/* static */ VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApplication::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    string color = COLOR_RESET;

    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        // Call tracking message
        color = COLOR_DIM;
    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        // Informational message like the creation of a resource
        color = COLOR_DIM_BLUE;
    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        // Message about behavior that is not necessarily an error, but very likely a bug in your application
        color = COLOR_YELLOW;
    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        // Message about behavior that is invalid and may cause crashes
        color = COLOR_RED;
    }

    cerr << color << COLOR_BOLD << "validation layer: " << COLOR_RESET << color << pCallbackData->pMessage << COLOR_RESET << "\n\n";

    return VK_FALSE;
}

/* static */ VkResult HelloTriangleApplication::CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr) return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

/* static */ void HelloTriangleApplication::DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator) {

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != nullptr) func(instance, debugMessenger, pAllocator);
}

