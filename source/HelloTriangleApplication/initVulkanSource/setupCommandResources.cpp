#include <header.hpp>

void HelloTriangleApplication::setupCommandResources() {
    createCommandPool();
    createCommandBuffer();
}

void HelloTriangleApplication::createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(_physicalDevice, VK_QUEUE_GRAPHICS_BIT);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(_device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
        throw std::runtime_error("failed to create command pool!");
}

void HelloTriangleApplication::createCommandBuffer() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(_device, &allocInfo, &_commandBuffer) != VK_SUCCESS)
        throw std::runtime_error("failed to allocate command buffers!");
}
