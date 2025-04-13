#pragma once

#define VKAPI_CALLBACK VKAPI_ATTR VkBool32 VKAPI_CALL

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
