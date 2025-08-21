#include <header.hpp>

void ScopApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}
