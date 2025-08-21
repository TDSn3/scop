#include <header.hpp>

void ScopApplication::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    // Fin de l'ecriture des commandes dans le buffer
	vkEndCommandBuffer(commandBuffer);

	// Execution du tampon de commandes (CommandBuffer)

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_graphicsQueue);

    vkFreeCommandBuffers(_device, _commandPool, 1, &commandBuffer);
}
