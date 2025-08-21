#include <header.hpp>

void ScopApplication::createTextureImageView() {
	_textureImageView = createImageView(_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}
