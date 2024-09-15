#include <header.hpp>

void HelloTriangleApplication::createSurface() {
    VkResult result = glfwCreateWindowSurface(_instance, _window, nullptr, &_surface);
    
    if (result != VK_SUCCESS)
        throw runtime_error("failed to create window surface!");
}
