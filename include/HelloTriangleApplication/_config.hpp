#pragma once

#define VKAPI_CALLBACK VKAPI_ATTR VkBool32 VKAPI_CALL

#ifdef NDEBUG
 const bool enableValidationLayers = false;
#else
 const bool enableValidationLayers = true;
#endif

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2; // Nombre d'images à traiter simultanément

const vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation",
};

const vector<const char*> physicalDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

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

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        // Attribut 1: Position (vec2)
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT; // vec2
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        // Attribut 2: Couleur (vec3)
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT; // vec3
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

// const vector<Vertex> vertices = {
//     {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
//     {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
//     {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
// };

// const vector<Vertex> vertices = {
//     {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
//     {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
//     {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
// };

const vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};
