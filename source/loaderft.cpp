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

bool loadOBJ(
    const string &objPath,
    const string &mtlDir,
    vector<Vertex> &vertices,
    vector<uint32_t> &indices) {

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
            Vec3 v;
            
            iss >> v.x >> v.y >> v.z; positions.push_back(v);
        } else if (tag == "vt") {
            Vec2 vt;
            
            iss >> vt.x >> vt.y; texcoords.push_back(vt);
        } else if (tag == "mtllib") {
            string mtlFile;
            
            iss >> mtlFile;
            materials = loadMaterials(mtlDir + "/" + mtlFile);
        } else if (tag == "usemtl") {
            iss >> currentMtl;
        } else if (tag == "f") {
            vector<string> verts;
            string vtoken;

            while (iss >> vtoken) verts.push_back(vtoken);

            vector<uint32_t> faceLocal; 
            faceLocal.reserve(verts.size());

            for (const string &vtok : verts) {
                istringstream viss(vtok);
                string vStr, tStr, nStr;
                getline(viss, vStr, '/');
                getline(viss, tStr, '/');
                getline(viss, nStr, '/');

                auto fetchPos = [&](int idx)->Vec3 {
                    if (idx > 0 && static_cast<size_t>(idx) <= positions.size())
                        return positions[idx - 1];
                    if (idx < 0 && static_cast<size_t>(-idx) <= positions.size())
                        return positions[positions.size() + idx];
                    throw runtime_error("position index out of range");
                };
                auto fetchUV = [&](int idx)->Vec2 {
                    if (idx > 0 && static_cast<size_t>(idx) <= texcoords.size())
                        return { texcoords[idx - 1].x, 1.0f - texcoords[idx - 1].y };
                    if (idx < 0 && static_cast<size_t>(-idx) <= texcoords.size())
                        return { texcoords[texcoords.size() + idx].x,
                                1.0f - texcoords[texcoords.size() + idx].y };
                    return Vec2{0.0f, 0.0f};
                };

                int vi = stoi(vStr);
                int ti = tStr.empty() ? 0 : stoi(tStr);

                Vertex vert{};
                vert.pos = fetchPos(vi);
                vert.texCoord = fetchUV(ti);

                auto it = materials.find(currentMtl);
                vert.color = (it != materials.end()) ? it->second.diffuse : Vec3{1.0f,1.0f,1.0f};

                vertices.push_back(vert);
                faceLocal.push_back(static_cast<uint32_t>(vertices.size() - 1));
            }

            if (faceLocal.size() >= 3) {
                for (size_t k = 1; k + 1 < faceLocal.size(); ++k) {
                    indices.push_back(faceLocal[0]);
                    indices.push_back(faceLocal[k]);
                    indices.push_back(faceLocal[k + 1]);
                }
            }
        }
    }
    return true;
}

#endif
