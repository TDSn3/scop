#pragma once

using namespace std;

struct QueueFamilyIndices {
    optional<uint32_t> graphicsFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

class HelloTriangleApplication {
public:

    // run.cpp
    void run();

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    const vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

private:

    // run.cpp
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    // main.cpp
    void createVulkanInstance ();
    bool checkValidationLayerSupport();
    vector<const char *> getRequiredGlfwExtensions();
    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkQueueFlagBits flags);
    void createLogicalDevice();
    vector<const char *> getRequiredLogicalDeviceExtensions();

    // setupDebugMessenger.cpp
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
    static VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugUtilsMessengerEXT *pDebugMessenger);
    static void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks *pAllocator);

    // print.cpp
    void printValidationLayerSupport();
    void printSupportedPhysicalDeviceExtensions();
    void printAvailableVulkanExtension();
    void printAvailableGlfwExtension(vector<const char *> &glfwExtensions);
    void printQueueFamilies(VkPhysicalDevice device);

    GLFWwindow *_window;
    VkInstance _instance;
    VkDebugUtilsMessengerEXT _debugMessenger;
    VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
    VkDevice _device;
    VkQueue _graphicsQueue;
};
