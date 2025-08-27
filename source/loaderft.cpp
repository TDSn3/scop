#ifndef USE_LOADLIB

#include "loaderft.hpp"

bool loadImage(const string &path, int &width, int &height, vector<unsigned char> &pixels) {
    ifstream file(path, ios::binary);
    if (!file) {
        cerr << "Failed to open image: " << path << "\n";
        return false;
    }

    string magic;
    file >> magic;
    if (magic != "P6") {
        cerr << "Unsupported format (expected binary PPM P6): " << magic << "\n";
        return false;
    }

    int maxval;
    file >> width >> height >> maxval;
    file.ignore(1);

    if (maxval != 255) {
        cerr << "Unsupported maxval (must be 255): " << maxval << "\n";
        return false;
    }

    size_t sizeRGB = static_cast<size_t>(width) * height * 3;
    vector<unsigned char> rgb(sizeRGB);
    file.read(reinterpret_cast<char*>(rgb.data()), sizeRGB);

    if (!file) {
        cerr << "Failed to read PPM pixel data\n";
        return false;
    }

    pixels.resize(static_cast<size_t>(width) * height * 4);
    for (int i = 0, j = 0; i < width * height; i++, j += 3) {
        pixels[i * 4 + 0] = rgb[j + 0]; // R
        pixels[i * 4 + 1] = rgb[j + 1]; // G
        pixels[i * 4 + 2] = rgb[j + 2]; // B
        pixels[i * 4 + 3] = 255;        // A
    }

    return true;
}

struct Material { Vec3 diffuse{1.0f,1.0f,1.0f}; };

static unordered_map<string, Material> loadMaterials(const string &path) {
    unordered_map<string, Material> mats;
    ifstream file(path);
    if (!file) return mats;
    string line, current;
    while (getline(file, line)) {
        istringstream iss(line);
        string tag; iss >> tag;
        if (tag == "newmtl") {
            iss >> current; mats[current] = Material{};
        } else if (tag == "Kd") {
            float r,g,b; iss >> r >> g >> b;
            mats[current].diffuse = {r,g,b};
        }
    }
    return mats;
}

bool loadOBJ(const string &objPath, const string &mtlDir,
             vector<Vertex> &vertices, vector<uint32_t> &indices) {
    ifstream file(objPath);
    if (!file) return false;

    vector<Vec3> positions;
    vector<Vec2> texcoords;
    unordered_map<string, Material> materials;
    string line;
    string currentMtl;

    while (getline(file, line)) {
        istringstream iss(line);
        string tag; iss >> tag;
        if (tag == "v") {
            Vec3 v; iss >> v.x >> v.y >> v.z; positions.push_back(v);
        } else if (tag == "vt") {
            Vec2 vt; iss >> vt.x >> vt.y; texcoords.push_back(vt);
        } else if (tag == "mtllib") {
            string mtlFile; iss >> mtlFile;
            materials = loadMaterials(mtlDir + "/" + mtlFile);
        } else if (tag == "usemtl") {
            iss >> currentMtl;
        } else if (tag == "f") {
            vector<string> verts;
            string vstr;
            while (iss >> vstr) verts.push_back(vstr);

            for (const string &vtoken : verts) {
                istringstream viss(vtoken);
                string vStr, tStr, nStr;
                getline(viss, vStr, '/');
                getline(viss, tStr, '/');
                getline(viss, nStr, '/');

                int vi = stoi(vStr);
                int ti = tStr.empty() ? 0 : stoi(tStr);

                Vertex vert{};

                if (vi > 0 && static_cast<size_t>(vi) <= positions.size())
                    vert.pos = positions[vi - 1];
                else if (vi < 0 && static_cast<size_t>(-vi) <= positions.size())
                    vert.pos = positions[positions.size() + vi];
                else
                    continue;

                if (ti > 0 && static_cast<size_t>(ti) <= texcoords.size())
                    vert.texCoord = { texcoords[ti - 1].x, 1.0f - texcoords[ti - 1].y };
                else
                    vert.texCoord = { 0.0f, 0.0f };

                auto it = materials.find(currentMtl);
                vert.color = (it != materials.end()) ? it->second.diffuse : Vec3{1.0f,1.0f,1.0f};

                vertices.push_back(vert);
                indices.push_back(static_cast<uint32_t>(indices.size()));
            }
        }
    }
    return true;
}

#endif
