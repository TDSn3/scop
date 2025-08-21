#include <header.hpp>

void ScopApplication::createDepthResources() {
    VkFormat depthFormat = findDepthFormat();

    createImage(
        _swapChainExtent.width,
        _swapChainExtent.height,
        depthFormat,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        _depthImage, 
        _depthImageMemory
    );

    _depthImageView = createImageView(_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}
