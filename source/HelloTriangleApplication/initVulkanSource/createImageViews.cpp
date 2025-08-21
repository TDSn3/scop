#include <header.hpp>

void HelloTriangleApplication::createImageViews() {
    size_t swapChainSize = _swapChainImages.size();

    _swapChainImageViews.resize(swapChainSize);

    for (size_t i = 0; i < swapChainSize; i++)
        _swapChainImageViews[i] = createImageView(_swapChainImages[i], _swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
}
