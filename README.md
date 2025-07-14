# Usage

### MacOS

GLFW configuration :

``` shell
mkdir ./library/glfw-3.4/build
cd ./library/glfw-3.4/build
cmake -DBUILD_SHARED_LIBS=OFF ..
make
cd ../../..
```

# Utils

### Validation layer

To activate the validation layer, remove `-DNDEBUG` from `CFLAGS` in the **Makefile**.

### Resources

- A Comparison of Modern Graphics APIs : https://alain.xyz/blog/comparison-of-modern-graphics-apis
- Setup Vulkan on MacOS : https://youtu.be/C7YgtkR9KLg?si=v9-MUbwKavMHITeh
- Getting Started with the macOS Vulkan SDK : https://vulkan.lunarg.com/doc/view/1.3.268.1/mac/getting_started.html
- Tutorial Vulkan : https://vulkan-tutorial.com/

### Library

- `SDK Vulkan` Development environment : https://www.lunarg.com/vulkan-sdk/
- `GLM` Linear algebra operations : https://github.com/g-truc/glm
- `GLFW` Window creation : https://www.glfw.org/
