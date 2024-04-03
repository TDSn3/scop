#pragma once

#define VKAPI_CALLBACK VKAPI_ATTR VkBool32 VKAPI_CALL

using namespace std;

struct QueueFamilyIndices {
    optional<uint32_t> graphicsFamily;
    optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR    capabilities;
    vector<VkSurfaceFormatKHR>  formats;
    vector<VkPresentModeKHR>    presentModes;
};

class HelloTriangleApplication {
public:

    const int WIDTH = 800;
    const int HEIGHT = 600;
    const vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };
    const vector<const char*> physicalDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    // run.cpp
    void run();

private:

    GLFWwindow                  *_window;
    VkInstance                  _instance;
    VkDebugUtilsMessengerEXT    _debugMessenger;
    VkSurfaceKHR                _surface;
    VkPhysicalDevice            _physicalDevice = VK_NULL_HANDLE;
    VkDevice                    _device;
    VkQueue                     _graphicsQueue;
    VkQueue                     _presentQueue;

    // run.cpp
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    // runMain.cpp
    void                    createVulkanInstance ();
    bool                    checkValidationLayerSupport();
    vector<const char *>    getRequiredGlfwExtensions();
    void                    pickPhysicalDevice();
    bool                    isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices      findQueueFamilies(VkPhysicalDevice device, VkQueueFlagBits flags);
    bool                    checkPhysicalDeviceExtensionSupport(VkPhysicalDevice device);
    void                    createLogicalDevice();
    vector<const char *>    getRequiredLogicalDeviceExtensions();
    void                    createSurface();
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    // setupDebugMessenger.cpp
    void                    setupDebugMessenger();
    void                    populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    static VKAPI_CALLBACK   debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    static VkResult         CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
    static void             DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

    // print.cpp
    void printValidationLayerSupport();
    void printAvailableVulkanExtension();
    void printAvailableGlfwExtension(vector<const char *> &glfwExtensions);
    void printAvailablePhysicalDevices();
    void printQueueFamilies(VkPhysicalDevice device);
    void printQueueFamilyIndices(const QueueFamilyIndices &indices);
    void printSupportedPhysicalDeviceExtensions();

};
