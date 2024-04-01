#include <header.hpp>

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr) return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator) {

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != nullptr) func(instance, debugMessenger, pAllocator);
}

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    const vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

private:
    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        _window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }
    
    void initVulkan() {
        createVulkanInstance();
        setupDebugMessenger();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(_window)) glfwPollEvents();
    }

    void cleanup() {
        DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
        vkDestroyInstance(_instance, nullptr);
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    void createVulkanInstance () {
        if (!checkValidationLayerSupport())
            throw runtime_error("validation layers requested, but not available!");

        VkInstanceCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;

        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; // MacOS

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

        createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());;
        createInfo.ppEnabledExtensionNames = glfwExtensions.data();

        printAvailableVulkanExtension();
        printAvailableGlfwExtension(glfwExtensions);

        VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);

        if (result != VK_SUCCESS)
            throw runtime_error("failed to create Vulkan instance: error " + to_string(result) + "\n"); // error code : https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkResult.html
    }

    void printAvailableVulkanExtension() {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        vector<VkExtensionProperties> extensions(extensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        cout << extensions.size() << " available Vulkan extensions:\n";

        for (const auto &extension : extensions)
            cout << "\t" << extension.extensionName << "\n";
    }

    void printAvailableGlfwExtension(vector<const char *> &glfwExtensions) {
        cout << glfwExtensions.size() << " available GLFW extensions:\n";

        for (size_t i = 0; i < glfwExtensions.size(); i++)
            cout << "\t" << glfwExtensions[i] << "\n";
    }

    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        cout << availableLayers.size() << " available layers:\n";

        for (const auto &layerProperties : availableLayers)
            cout << "\t" << layerProperties.layerName << "\n";

        cout << validationLayers.size() << " validation layers:\n";

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

        return true;
    }

    vector<const char *> getRequiredGlfwExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        vector<const char*> glfwExtensionsExtend(glfwExtensions, glfwExtensions + glfwExtensionCount);

        #ifdef __APPLE__
            glfwExtensionsExtend.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        #endif

        glfwExtensionsExtend.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        return glfwExtensionsExtend;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
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

    void setupDebugMessenger() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;

        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS)
            throw std::runtime_error("failed to set up debug messenger!");
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    GLFWwindow *_window;
    VkInstance _instance;
    VkDebugUtilsMessengerEXT _debugMessenger;
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const exception &e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
