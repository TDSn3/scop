# HelloTriangleApplication

## 1. **Structure générale**  
   - Le fichier d’en-tête ` HelloTriangleApplication.hpp ` déclare une classe avec toutes ses fonctions membres et ses attributs Vulkan (instance, device, swapchain, etc.).  
   - Le fichier ` run.cpp ` contient l’implémentation des fonctions principales : ` run() `, ` initWindow() `, ` initVulkan() `, ` mainLoop() `, ` drawFrame() ` et ` cleanup() `.  

   La fonction ` run() ` encapsule le cycle de vie complet de l’application :
   1. Initialisation de la fenêtre (` initWindow() `)  
   2. Initialisation de Vulkan (` initVulkan() `)  
   3. Boucle principale de rendu (` mainLoop() `)  
   4. Nettoyage final (` cleanup() `)

## 2. **` run() `**  
   - C’est la fonction « moteur » de la classe.  
   - Appelle successivement :  
     1. ` initWindow() ` : crée la fenêtre GLFW, configure les hints (pas de contexte OpenGL, fenêtre non redimensionnable, etc.).  
     2. ` initVulkan() ` : crée et configure toutes les ressources Vulkan (explications ci-dessous).  
     3. ` mainLoop() ` : lance la boucle d’événements + rendu.  
     4. ` cleanup() ` : détruit toutes les ressources (Vulkan, fenêtre, etc.).

## 3. **` initWindow() `**  
   - ` glfwInit() ` : initialise la bibliothèque GLFW (gestion de la fenêtre, de l’entrée clavier/souris).  
   - ` glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API) ` : indique qu’on n’utilise pas OpenGL, mais Vulkan.  
   - ` glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE) ` : rend la fenêtre non redimensionnable (pour simplifier).  
   - ` glfwCreateWindow(...) ` : crée la fenêtre. On garde le pointeur dans ` _window `.

## 4. **` initVulkan() `**  
   C’est la partie la plus importante, où l’on procède à toutes les étapes d’initialisation Vulkan. Voici les appels, dans l’ordre :  
   1. ` createVulkanInstance() `  
      - Crée l’instance Vulkan (objet de plus haut niveau qui « encadre » l’utilisation de Vulkan).  
      - C’est ici qu’on spécifie notamment les extensions qu’on veut utiliser (ex. ` VK_KHR_surface ` pour l’affichage, etc.) et les couches de validation (` validation layers `) si on les active en debug.  
   2. ` setupDebugMessenger() `  
      - Configure le callback de debug, pour que Vulkan nous envoie des messages en cas d’erreurs, d’avertissements, etc. (utile en debug).  
   3. ` createSurface() `  
      - Crée la surface de rendu associée à la fenêtre GLFW. Cette surface est ensuite utilisée par Vulkan pour présenter les images (swapchain).  
   4. ` pickPhysicalDevice() `  
      - Sélectionne la carte graphique (ou l’IGP) qui répond le mieux à nos besoins Vulkan (support des extensions, performance, etc.).  
      - Retient la référence dans ` _physicalDevice `.  
   5. ` createLogicalDevice() `  
      - Sur le GPU physique choisi, crée un *logical device* (objet Vulkan) qui nous donne accès aux files (queues) de commandes.  
      - Récupère en particulier ` _graphicsQueue ` et ` _presentQueue ` (souvent elles sont identiques ou différentes selon le GPU / la plateforme).  
   6. ` createSwapChain() `  
      - Crée le swapchain Vulkan, c’est-à-dire un ensemble d’images (les buffers d’écran) qu’on utilisera pour l’affichage.  
      - On détermine le format de couleur (` _swapChainImageFormat `) et la taille (` _swapChainExtent `).  
   7. ` createImageViews() `  
      - Pour chaque image du swapchain, on crée un *image view*, qui est la vue sous laquelle on va lire/écrire la texture quand on dessine.  
   8. ` createRenderPass() `  
      - Déclare l’*attachment* de rendu (en gros, quelles images sont rendues, dans quel format, avec quelles opérations de chargement/effacement).  
      - Un *render pass* décrit le déroulement et la configuration du rendu (couleur, profondeur, etc.).  
   9. ` createGraphicsPipeline() `  
      - Crée le pipeline de rendu : stages de shaders (vertex, fragment), configuration du viewport et du scissor, configuration de l’assemblage, etc.  
      - Le pipeline, en Vulkan, est un objet plus ou moins immuable qui regroupe les états de configuration du GPU.  
   10. ` createFramebuffers() `  
       - Pour chaque image du swapchain, crée un *framebuffer* qui associe l’image vue (image view) au render pass.  
       - Un framebuffer concrétise l’interface entre l’image et le render pass.  
   11. ` setupCommandResources() `  
       - On voit à l’intérieur qu’il y a notamment ` createCommandPool() ` et ` createCommandBuffer() `.  
       - Le *command pool* est l’objet Vulkan depuis lequel on va allouer des *command buffers*.  
       - Le *command buffer* contient les commandes pour dessiner.  
   12. ` createSyncObjects() `  
       - Crée des sémaphores, fences et autres objets de synchronisation.  
       - Permet de synchroniser le CPU et le GPU (ex. s’assurer qu’on ne dessine pas sur une image qui est encore en cours de présentation à l’écran).  

## 5. **` mainLoop() `**  
   - Boucle tant que la fenêtre n’est pas fermée (` !glfwWindowShouldClose(_window) `).  
   - ` glfwPollEvents() ` : traite les événements (clavier, souris, etc.).  
   - ` drawFrame() ` : étape de dessin (ou affichage).  
   - À la fin de la boucle, on attend que le ` device ` soit « idle » (plus aucune opération GPU en cours) avec ` vkDeviceWaitIdle(_device) `.  

## 6. **` drawFrame() `**  
   C’est ici que ça se passe image par image :  
   1. ` vkWaitForFences(...) / vkResetFences(...) `  
      - Attend que le GPU ait fini la frame précédente (via la fence).  
      - Reset la fence pour la réutiliser.  
   2. ` vkAcquireNextImageKHR(...) `  
      - Récupère l’index d’une image libre dans le swapchain.  
      - On passe en paramètres un sémaphore (` _imageAvailableSemaphore `) qui sera signalé quand l’image sera prête.  
   3. ` vkResetCommandBuffer(...) ` + ` recordCommandBuffer(...) `  
      - Réinitialise le command buffer.  
      - Enregistre les commandes de dessin (begin render pass, draw, end render pass, etc.) dans la fonction ` recordCommandBuffer(...) `.  
   4. **Submit au GPU**  
      - Remplit une structure ` VkSubmitInfo ` qui dit :  
        - « Attends tel sémaphore (` _imageAvailableSemaphore `) avant d’exécuter les commandes. »  
        - « Voici mon command buffer à exécuter. »  
        - « Signal tel sémaphore (` _renderFinishedSemaphore `) quand c’est fini. »  
      - ` vkQueueSubmit(_graphicsQueue, ...) ` envoie le tout à la *graphics queue*.  
      - On associe la *fence* (` _inFlightFence `) pour être notifié quand tout est fini.  
   5. **Présenter l’image à l’écran**  
      - ` vkQueuePresentKHR(...) ` en utilisant le sémaphore ` _renderFinishedSemaphore ` comme condition d’attente.  
      - Demande d’afficher l’image indexée par ` imageIndex `.  

## 7. **` cleanup() `**  
   - Détruit tous les objets Vulkan dans l’ordre inverse de leur création (c’est important en Vulkan) : sémaphores, fence, command pool, framebuffers, pipeline, render pass, image views, swapchain, device, etc.  
   - Détruit l’instance Vulkan et le debug messenger.  
   - Détruit la surface.  
   - Nettoie la fenêtre GLFW (` glfwDestroyWindow(_window) `) et termine GLFW (` glfwTerminate() `).  

-----

## Exemple

``` mermaid

flowchart TB
    A["main()"] --> B["HelloTriangleApplication::run()"]
    B --> C["initWindow()"]
    B --> D["initVulkan()"]
    D --> E["createVulkanInstance()"]
    D --> F["setupDebugMessenger()"]
    D --> G["createSurface()"]
    D --> H["pickPhysicalDevice()"]
    D --> I["createLogicalDevice()"]
    D --> J["createSwapChain()"]
    D --> K["createImageViews()"]
    D --> L["createRenderPass()"]
    D --> M["createGraphicsPipeline()"]
    D --> N["createFramebuffers()"]
    D --> O["setupCommandResources()"]
    D --> P["createSyncObjects()"]
    B --> Q["mainLoop()"]
    Q --> R["drawFrame()"]
    R --> Q
    Q --> S["vkDeviceWaitIdle()"]
    B --> T["cleanup()"]

```

``` mermaid

flowchart LR
    subgraph AINIT [Initialization]
        direction TB
        A["Create Vulkan Instance"]
        B["Setup Debug Messenger"]
        C["Create Surface"]
        D["Pick Physical Device"]
        E["Create Logical Device & Queues"]
    end

    subgraph BSWAP [Swapchain & Pipeline]
        direction TB
        F["Create Swapchain & Image Views"]
        G["Create Render Pass"]
        H["Create Graphics Pipeline"]
        I["Create Framebuffers"]
        J["Create Command Pool & Buffers"]
        K["Create Sync Objects"]
    end

    subgraph CLOOP [Main Loop]
        direction TB
        L["drawFrame() : Acquire -> Record -> Submit -> Present"]
    end

    subgraph DCLEAN [Cleanup]
        direction TB
        M["Destroy All Resources"]
    end

    AINIT --> BSWAP --> CLOOP --> DCLEAN

    %% Annotations (lignes pointillées)

    A -. "L'instance établit la connexion entre l'application et Vulkan." .- A
    B -. "Le Debug Messenger gère les messages de validation et de débogage." .- B
    C -. "La surface relie Vulkan au système de fenêtrage (GLFW, etc.)." .- C
    D -. "Sélection du GPU adapté aux besoins (extensions, performances...)." .- D
    E -. "Le device logique & les queues permettent d'envoyer des commandes au GPU." .- E

    F -. "La swapchain regroupe les images d'affichage (double/triple buffering)." .- F
    G -. "Le render pass décrit comment ces images sont rendues (attachments, subpasses...)." .- G
    H -. "Le pipeline configure les shaders, l'assemblage, le viewport, etc." .- H
    I -. "Le framebuffer associe le render pass aux images de la swapchain." .- I
    J -. "Les command buffers contiennent les commandes de dessin." .- J
    K -. "Objets de synchro (sémaphores, fences) coordonnent CPU et GPU." .- K

    L -. "Boucle principale : acquérir l'image, enregistrer les commandes, soumettre, présenter." .- L
    M -. "Détruit toutes les ressources Vulkan en sens inverse de leur création." .- M

```