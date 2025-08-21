#include <header.hpp>

void ScopApplication::createVertexBuffer() {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    // stagingBuffer : buffer alloué sur le CPU pour y copier les vertices
    createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory
    );

    void* data;

    vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(_device, stagingBufferMemory);

    // vertexBuffer : "vrai" buffer alloué sur le GPU (memoire plus rapide sur le GPU que sur le CPU)
    createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        _vertexBuffer, 
        _vertexBufferMemory);

    copyBuffer(stagingBuffer, _vertexBuffer, bufferSize);

    // Destruction du stagingBuffer apres la copie des vertices sur le vertexBuffer
    vkDestroyBuffer(_device, stagingBuffer, nullptr);
    vkFreeMemory(_device, stagingBufferMemory, nullptr);
}

// Chaque carte graphique propose plusieurs types de mémoire aux propriétés et performances différentes.
// Nous devons sélectionner un type adapté à nos besoins spécifiques (tampon et application).
// Cette fonction permet d'effectuer ce choix en combinant ces contraintes.
uint32_t ScopApplication::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties); //  interroge les informations sur les types de mémoire disponibles 

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw runtime_error("failed to find suitable memory type!");
}
