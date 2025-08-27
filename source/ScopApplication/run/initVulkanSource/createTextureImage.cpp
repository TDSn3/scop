#include <header.hpp>

void ScopApplication::createTextureImage() {
    int texWidth = 1;
    int texHeight = 1;

#ifdef USE_LOADLIB

    int texChannels = 4;

    stbi_uc whitePixel[4] = {255, 255, 255, 255};
    stbi_uc* pixels = whitePixel;

    bool hasTexture = !_texturePath.empty();

    if (hasTexture) {
        ifstream file(_texturePath);

        if (file) {
            pixels = stbi_load(_texturePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
            file.close();

            if (!pixels)
                throw runtime_error("failed to load texture image!");
        } else {
            hasTexture = false;
        }
    }

    VkDeviceSize imageSize = texWidth * texHeight * 4;

#else

        std::vector<unsigned char> pixels(4, 255);

    if (!_texturePath.empty())
        if (!loadImage(_texturePath, texWidth, texHeight, pixels))
            cerr << "Failed to load texture '" << _texturePath << "', using fallback color\n";

    VkDeviceSize imageSize = static_cast<VkDeviceSize>(texWidth) * texHeight * 4;

#endif

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(
        imageSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory
    );

    void *data;
    vkMapMemory(_device, stagingBufferMemory, 0, imageSize, 0, &data);

#ifdef USE_LOADLIB
    memcpy(data, pixels, static_cast<size_t>(imageSize));
#else
    memcpy(data, pixels.data(), static_cast<size_t>(imageSize));
#endif

#ifdef USE_LOADLIB
    if (hasTexture)
        stbi_image_free(pixels);
#endif

    vkUnmapMemory(_device, stagingBufferMemory);

    createImage(
        texWidth,
        texHeight,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        _textureImage,
        _textureImageMemory
    );

    transitionImageLayout(
        _textureImage,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    );

    copyBufferToImage(
        stagingBuffer,
        _textureImage,
        static_cast<uint32_t>(texWidth),
        static_cast<uint32_t>(texHeight)
    );

    transitionImageLayout(
        _textureImage,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );

    vkDestroyBuffer(_device, stagingBuffer, nullptr);
    vkFreeMemory(_device, stagingBufferMemory, nullptr);
}
