#ifndef HEADER_HPP
# define HEADER_HPP

# include <algorithm>
# include <array>
# include <cstdlib>
# include <cstring>
# include <fstream>
# include <functional>
# include <iostream>
# include <limits> 
# include <optional>
# include <set>
# include <stdexcept>
# include <string>
# include <vector>
# include <filesystem>

# define GLFW_INCLUDE_VULKAN
# include <GLFW/glfw3.h>

# include <chrono>

# ifdef USE_LOADLIB
#  include <stb_image.h>
#  include <tiny_obj_loader.h>
#  else
#  include "loaderft.hpp"
# endif

# include "ScopApplication/ScopApplication.hpp"

# include "colors.hpp"

using namespace std;

#endif
