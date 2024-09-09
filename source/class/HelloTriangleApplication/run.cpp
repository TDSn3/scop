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
	createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
	createSwapChain();
    createImageViews();
    createGraphicsPipeline();
}

void HelloTriangleApplication::mainLoop() {
    while (!glfwWindowShouldClose(_window)) glfwPollEvents();
}

void HelloTriangleApplication::cleanup() {
   for (auto imageView : _swapChainImageViews)
        vkDestroyImageView(_device, imageView, nullptr);

	vkDestroySwapchainKHR(_device, _swapChain, nullptr);
    vkDestroyDevice(_device, nullptr);
    DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
	vkDestroySurfaceKHR(_instance, _surface, nullptr);
    vkDestroyInstance(_instance, nullptr);
    glfwDestroyWindow(_window);
    glfwTerminate();
}
