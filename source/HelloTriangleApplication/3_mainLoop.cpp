#include <header.hpp>

void HelloTriangleApplication::mainLoop() {
    while (!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(_device);
}

void HelloTriangleApplication::drawFrame() {
    // 1) Synchronisation : attendre la fin du GPU sur la frame précédente
    vkWaitForFences(_device, 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

    // 2) Acquisition d’une image de la swapchain
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized) {
        vkDeviceWaitIdle(_device);

        vkDestroySemaphore(_device, _imageAvailableSemaphores[_currentFrame], nullptr);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_imageAvailableSemaphores[_currentFrame]) != VK_SUCCESS)
            throw runtime_error("failed to create semaphore!");

        recreateSwapChain();
        _framebufferResized = false;

        return; // Saute cette frame

        // Version original avec un bug :
        //
        // recreateSwapChain();
        // _framebufferResized = false;
        // return;
    } else if (result != VK_SUCCESS) {
        throw runtime_error("failed to acquire swap chain image!");
    }

    // Mise à jour des données uniformes
    updateUniformBuffer(_currentFrame);

    // Ne réinitialiser "le fence" que si nous soumettons un nouvelle objet
    vkResetFences(_device, 1, &_inFlightFences[_currentFrame]);

    // 3) Réinitialisation et enregistrement du command buffer
    vkResetCommandBuffer(_commandBuffers[_currentFrame], 0);
    recordCommandBuffer(_commandBuffers[_currentFrame], imageIndex);

    // 4) Préparation des infos pour soumettre le command buffer au GPU
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    // Sémaphores d’attente : on attend que l’image soit prête (signalée par _imageAvailableSemaphores)
    VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores[_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    // Le command buffer à exécuter
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_commandBuffers[_currentFrame];

    // Sémaphores de signal : une fois le dessin fini, on signale _renderFinishedSemaphores
    VkSemaphore signalSemaphores[] = {_renderFinishedSemaphores[_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Soumission à la file graphique (graphics queue)
    if (vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS)
        throw std::runtime_error("failed to submit draw command buffer!");

    // 5) Présentation de l’image rendue
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    // Attend que le rendu soit fini (_renderFinishedSemaphores) avant de présenter
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    // Quelle swapchain utiliser et quelle image présenter
    VkSwapchainKHR swapChains[] = {_swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    // Demande au GPU d’afficher l’image
    result = vkQueuePresentKHR(_presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw runtime_error("failed to present swap chain image!");
    }

    // Passe à l'image suivante 
    _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
