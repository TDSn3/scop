#pragma once

#include "_config.hpp"

class HelloTriangleApplication {
public:

    // ⤹ run.cpp
        void                        run();

private:

    GLFWwindow                      *_window;

    VkInstance                      _instance;
    VkDebugUtilsMessengerEXT        _debugMessenger;
    VkSurfaceKHR                    _surface;

    VkPhysicalDevice                _physicalDevice = VK_NULL_HANDLE;
    VkDevice                        _device; // logical device

    VkQueue                         _graphicsQueue;
    VkQueue                         _presentQueue;

    VkSwapchainKHR                  _swapChain;
    vector<VkImage>                 _swapChainImages;
    VkFormat                        _swapChainImageFormat;
    VkExtent2D                      _swapChainExtent;
    vector<VkImageView>             _swapChainImageViews;
    vector<VkFramebuffer>           _swapChainFramebuffers;

    VkRenderPass                    _renderPass;
    VkDescriptorSetLayout           _descriptorSetLayout;
    VkPipelineLayout                _pipelineLayout;
    VkPipeline                      _graphicsPipeline;

    VkCommandPool                   _commandPool;

    VkBuffer                        _vertexBuffer;
    VkDeviceMemory                  _vertexBufferMemory;
    VkBuffer                        _indexBuffer;
    VkDeviceMemory                  _indexBufferMemory;  

    vector<VkBuffer>                _uniformBuffers;
    vector<VkDeviceMemory>          _uniformBuffersMemory;
    vector<void *>                  _uniformBuffersMapped;

    // Chaque frame doit avoir son propre command buffer, ses propres sémaphores et sa propre fence (barrière).
    // vector permet de créer le nombre d'objets nécessaires pour chaque frame.
    // Voir `MAX_FRAMES_IN_FLIGHT` dans `./_config.hpp` pour connaître le nombre maximum d'images pouvant être gérées simultanément.
    vector<VkCommandBuffer>         _commandBuffers;

    vector<VkSemaphore>             _imageAvailableSemaphores;
    vector<VkSemaphore>             _renderFinishedSemaphores;
    vector<VkFence>                 _inFlightFences;

    bool                            _framebufferResized = false;

    // Pour utiliser les bons objets à chaque image, nous devons suivre l'image courante.
    uint32_t                        _currentFrame = 0;

    // ⤹ run.cpp
        void                        initWindow();
        static void                 framebufferResizeCallback(GLFWwindow* window, int width, int height);
        void                        initVulkan();
        void                        mainLoop();
        void                        drawFrame();
        void                        cleanup();

    // ⤹ initVulkanSource/
        void                        createVulkanInstance ();
        bool                        checkValidationLayerSupport();
        vector<const char *>        getRequiredGlfwExtensions();

        void                        setupDebugMessenger();
        void                        populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        static VKAPI_CALLBACK       debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        static VkResult             CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
        static void                 DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

        void                        createSurface();

        void                        pickPhysicalDevice();

        void                        createLogicalDevice();
        vector<const char *>        getRequiredLogicalDeviceExtensions();

        void                        recreateSwapChain();
        void                        cleanupSwapChain();

        void                        createSwapChain();
        SwapChainSupportDetails     querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR          chooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR            chooseSwapPresentMode(const vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D                  chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        void                        createImageViews();

        void                        createRenderPass();

        void                        createGraphicsPipeline();
        static vector<char>         readFile(const string &filename);
        VkShaderModule              createShaderModule(const vector<char> &code);

        void                        createFramebuffers();

        void                        setupCommandResources();
        void                        createCommandPool();
        void                        createCommandBuffers();

        void                        createSyncObjects();

        void                        createVertexBuffer();
        void                        createIndexBuffer();

        void                        createUniformBuffers();
        void                        updateUniformBuffer(uint32_t currentImage);

        void                        createDescriptorSetLayout();
        void                        createBuffer(VkDeviceSize size,VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
        uint32_t                    findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void                        copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    // ⤹ initVulkanSource/utils.cpp
        bool                        isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices          findQueueFamilies(VkPhysicalDevice device, VkQueueFlagBits flags);
        bool                        checkPhysicalDeviceExtensionSupport(VkPhysicalDevice device);

        void                        recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        void                        printValidationLayerSupport();
        void                        printAvailableVulkanExtension();
        void                        printAvailableGlfwExtension(vector<const char *> &glfwExtensions);
        void                        printAvailablePhysicalDevices();
        void                        printQueueFamilies(VkPhysicalDevice device);
        void                        printQueueFamilyIndices(const QueueFamilyIndices &QFindices);
        void                        printSupportedPhysicalDeviceExtensions();

};
