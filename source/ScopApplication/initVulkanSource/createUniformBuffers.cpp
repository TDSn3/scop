#include <header.hpp>

void ScopApplication::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    _uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    _uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    _uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _uniformBuffers[i], _uniformBuffersMemory[i]);

        vkMapMemory(_device, _uniformBuffersMemory[i], 0, bufferSize, 0, &_uniformBuffersMapped[i]);
    }
}
