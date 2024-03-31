#include <header.hpp>

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    const uint32_t  WIDTH = 800;
    const uint32_t  HEIGHT = 600;

private:
    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        _window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }
    
    void initVulkan() {
        createInstance();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(_window)) glfwPollEvents();
    }

    void cleanup() {
        vkDestroyInstance(_instance, nullptr);
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    void createInstance () {
        VkApplicationInfo appInfo{};

        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        //

        VkInstanceCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; // MacOS

        //

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        vector<const char*> glfwExtensionsExtend(glfwExtensions, glfwExtensions + glfwExtensionCount);

        #ifdef __APPLE__

        glfwExtensionsExtend.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        glfwExtensionCount++;

        #endif

        cout << glfwExtensionCount << " available GLFW extensions:\n";

        for (size_t i = 0; i < glfwExtensionCount; i++)
            cout << '\t' << glfwExtensionsExtend[i] << '\n';

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensionsExtend.data();

        //

        createInfo.enabledLayerCount = 0;

        //

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        vector<VkExtensionProperties> extensions(extensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        cout << extensions.size() << " available Vulkan extensions:\n";

        for (const auto& extension : extensions)
            cout << '\t' << extension.extensionName << '\n';

        //

        VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);
                
        if (result != VK_SUCCESS)
            throw runtime_error("failed to create Vulkan instance: error " + to_string(result) + "\n"); // error code : https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkResult.html
    }

    GLFWwindow *_window;
    VkInstance _instance;
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
