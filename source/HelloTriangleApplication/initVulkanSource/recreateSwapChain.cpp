#include <header.hpp>

//    Gère les changements de surface (ex. redimensionnement de fenêtre).
//     1.  Stoppe le GPU pour libérer l’ancienne swap‑chain.
//     2.  Détruit l’ancien set.
//     3.  Reconstruit la swap‑chain puis ses dépendances immédiates
//         (Image Views + Framebuffers). 
void HelloTriangleApplication::recreateSwapChain() {
    int width = 0;
    int height = 0;

    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(_window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(_device);  // 1

    cleanupSwapChain();         // 2

    createSwapChain();          // 3a
    createImageViews();         // 3b
    createDepthResources();
    createFramebuffers();       // 3c
}
