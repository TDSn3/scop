#include <header.hpp>

void ScopApplication::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    // Debut de l'ecriture des commandes dans le buffer

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    // Fin de l'ecriture des commandes dans le buffer
    // Execution du tampon de commandes (CommandBuffer)
    endSingleTimeCommands(commandBuffer);
}
