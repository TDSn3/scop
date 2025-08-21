#include <header.hpp>

void ScopApplication::pickPhysicalDevice() {
    printAvailablePhysicalDevices();
    
    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, nullptr);

    if (physicalDeviceCount == 0)
        throw runtime_error("failed to find GPUs with Vulkan support!");

    vector<VkPhysicalDevice> physicalDevice(physicalDeviceCount);
    vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, physicalDevice.data());

    for (const auto &physicalDeviceIterator : physicalDevice) {
        if (isDeviceSuitable(physicalDeviceIterator)) {
            _physicalDevice = physicalDeviceIterator;
            break;
        }
    }

    if (_physicalDevice == VK_NULL_HANDLE)
        throw runtime_error("failed to find a suitable GPU!");
}
