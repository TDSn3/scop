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

    _window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
}

/* static */ void HelloTriangleApplication::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    app->_framebufferResized = true;

    (void) width;
    (void) height;
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
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandPool();
    createTextureImage();
    createTextureImageView();
    createTextureSampler();
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
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

// 1
// void HelloTriangleApplication::cleanup() {
//     cleanupSwapChain();

//     vkDestroySampler(_device, _textureSampler, nullptr);
//     vkDestroyImageView(_device, _textureImageView, nullptr);
//     vkDestroyImage(_device, _textureImage, nullptr);
//     vkFreeMemory(_device, _textureImageMemory, nullptr);
// ////
//     vkDestroyBuffer(_device, _indexBuffer, nullptr);
//     vkFreeMemory(_device, _indexBufferMemory, nullptr);

//     vkDestroyBuffer(_device, _vertexBuffer, nullptr);
//     vkFreeMemory(_device, _vertexBufferMemory, nullptr);

//     for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//         vkDestroyBuffer(_device, _uniformBuffers[i], nullptr);
//         vkFreeMemory(_device, _uniformBuffersMemory[i], nullptr);
//     }

//     vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);

//     vkDestroyDescriptorSetLayout(_device, _descriptorSetLayout, nullptr);

//     vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
//     vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);

//     vkDestroyRenderPass(_device, _renderPass, nullptr);
// ////
//     for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//         vkDestroySemaphore(_device, _renderFinishedSemaphores[i], nullptr);
//         vkDestroySemaphore(_device, _imageAvailableSemaphores[i], nullptr);
//         vkDestroyFence(_device, _inFlightFences[i], nullptr);
//     }

//     vkDestroyCommandPool(_device, _commandPool, nullptr);

//     vkDestroyDevice(_device, nullptr);

//     if (enableValidationLayers)
//         DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);

//     vkDestroySurfaceKHR(_instance, _surface, nullptr);
//     vkDestroyInstance(_instance, nullptr);

//     glfwDestroyWindow(_window);

//     glfwTerminate();
// }

// 2
void HelloTriangleApplication::cleanup() {
    cleanupSwapChain();

    vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
    vkDestroyRenderPass(_device, _renderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(_device, _uniformBuffers[i], nullptr);
        vkFreeMemory(_device, _uniformBuffersMemory[i], nullptr);
    }

    vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);

    vkDestroySampler(_device, _textureSampler, nullptr);
    vkDestroyImageView(_device, _textureImageView, nullptr);

    vkDestroyImage(_device, _textureImage, nullptr);
    vkFreeMemory(_device, _textureImageMemory, nullptr);

    vkDestroyDescriptorSetLayout(_device, _descriptorSetLayout, nullptr);

    vkDestroyBuffer(_device, _indexBuffer, nullptr);
    vkFreeMemory(_device, _indexBufferMemory, nullptr);

    vkDestroyBuffer(_device, _vertexBuffer, nullptr);
    vkFreeMemory(_device, _vertexBufferMemory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(_device, _renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(_device, _imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(_device, _inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(_device, _commandPool, nullptr);

    vkDestroyDevice(_device, nullptr);

    if (enableValidationLayers)
        DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);

    vkDestroySurfaceKHR(_instance, _surface, nullptr);
    vkDestroyInstance(_instance, nullptr);

    glfwDestroyWindow(_window);

    glfwTerminate();
}

void HelloTriangleApplication::cleanupSwapChain() {
    for (size_t i = 0; i < _swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(_device, _swapChainFramebuffers[i], nullptr);
    }

    for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
        vkDestroyImageView(_device, _swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(_device, _swapChain, nullptr);
}
