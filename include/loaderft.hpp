#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <vulkan/vulkan.h>

#include "ScopApplication/ScopConfig.hpp"

using namespace std;

bool loadImage(const string &path, int &width, int &height, vector<unsigned char> &pixels);
bool loadOBJ(const string &objPath, const string &mtlDir, vector<Vertex> &vertices, vector<uint32_t> &indices);
