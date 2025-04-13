#include <header.hpp>

void HelloTriangleApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void HelloTriangleApplication::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::initVulkan() {
    createVulkanInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    setupCommandResources();
    createSyncObjects();
}

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
    vkResetFences(_device, 1, &_inFlightFences[_currentFrame]);

    // 2) Acquisition d’une image de la swapchain
    uint32_t imageIndex;
    vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);

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
    vkQueuePresentKHR(_presentQueue, &presentInfo);

    // Passe à l'image suivante 
    _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void HelloTriangleApplication::cleanup() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(_device, _renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(_device, _imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(_device, _inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(_device, _commandPool, nullptr);

    for (auto framebuffer : _swapChainFramebuffers)
        vkDestroyFramebuffer(_device, framebuffer, nullptr);

    vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
    vkDestroyRenderPass(_device, _renderPass, nullptr);

   for (auto imageView : _swapChainImageViews)
        vkDestroyImageView(_device, imageView, nullptr);

	vkDestroySwapchainKHR(_device, _swapChain, nullptr);
    vkDestroyDevice(_device, nullptr);
    DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
	vkDestroySurfaceKHR(_instance, _surface, nullptr);
    vkDestroyInstance(_instance, nullptr);
    glfwDestroyWindow(_window);
    glfwTerminate();
}
