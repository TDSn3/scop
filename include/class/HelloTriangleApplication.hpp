#pragma once

using namespace std;

class HelloTriangleApplication {
public:
    void run();

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    const vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

private:
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    void createVulkanInstance ();
    void printAvailableVulkanExtension();
    void printAvailableGlfwExtension(vector<const char *> &glfwExtensions);
    bool checkValidationLayerSupport();
    vector<const char *> getRequiredGlfwExtensions();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    GLFWwindow *_window;
    VkInstance _instance;
    VkDebugUtilsMessengerEXT _debugMessenger;
};
