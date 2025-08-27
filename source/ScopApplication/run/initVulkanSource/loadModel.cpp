#include <header.hpp>

void ScopApplication::loadModel() {

#ifdef USE_LOADLIB

    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string warn;
    string err;

    bool success = tinyobj::LoadObj(
        &attrib,
        &shapes,
        &materials,
        &warn,
        &err,
        _modelPath.c_str(),
        MTL_DIR_PATH 
    );

    if (!warn.empty())
        cerr << warn << "\n";
    if (!success)
        throw runtime_error(err.empty() ? "failed to load OBJ" : err);

    vertices.clear();
    indices.clear();

    size_t totalIdx = 0;

    for (const auto& s : shapes) totalIdx += s.mesh.indices.size();

    vertices.reserve(totalIdx);
    indices.reserve(totalIdx);

    for (const auto &shape : shapes) {
        size_t indexOffset = 0;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            const int fv = shape.mesh.num_face_vertices[f];

            Vec3 color(1.0f);

            int matId = (f < shape.mesh.material_ids.size()) ? shape.mesh.material_ids[f] : -1;

            if (matId >= 0 && matId < static_cast<int>(materials.size())) {
                const auto &mat = materials[matId];
                color = { mat.diffuse[0], mat.diffuse[1], mat.diffuse[2] }; // Kd
            }

            for (int v = 0; v < fv; ++v) {
                const tinyobj::index_t index = shape.mesh.indices[indexOffset + v];
                Vertex vertex{};

                const size_t vp = static_cast<size_t>(3 * index.vertex_index);
                if (vp + 2 < attrib.vertices.size()) {
                    vertex.pos = {
                        attrib.vertices[vp + 0],
                        attrib.vertices[vp + 1],
                        attrib.vertices[vp + 2]
                    };
                } else {
                    throw runtime_error("OBJ: vertex index out of range");
                }

                if (index.texcoord_index >= 0) {
                    const size_t u = static_cast<size_t>(2 * index.texcoord_index);
                    if (u + 1 < attrib.texcoords.size()) {
                        vertex.texCoord = {
                            attrib.texcoords[u + 0],
                            1.0f - attrib.texcoords[u + 1]
                        };
                    } else {
                        vertex.texCoord = {0.0f, 0.0f};
                    }
                } else {
                    vertex.texCoord = {0.0f, 0.0f};
                }

                vertex.color = color;

                vertices.push_back(vertex);
                indices.push_back(static_cast<uint32_t>(indices.size()));
            }
            indexOffset += fv;
        }
    }

#else

    if (!loadOBJ(_modelPath.string(), MTL_DIR_PATH, vertices, indices))
        throw runtime_error("failed to load OBJ");

#endif

}
