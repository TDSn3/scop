#include <header.hpp>

void HelloTriangleApplication::createTextureImageView() {
	_textureImageView = createImageView(_textureImage, VK_FORMAT_R8G8B8A8_SRGB);
}
