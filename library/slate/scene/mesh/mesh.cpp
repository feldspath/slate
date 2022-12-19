#include "mesh.hpp"

namespace slate {
    template<> Mesh<Vertex>::Mesh(const std::string& path) {
        ModelLoader model_loader(path);
        const RawMesh raw_mesh = model_loader.get_meshes()[0];

        std::vector<Vertex> vertices(raw_mesh.positions.size());
        int vertex_idx = 0;
        for (auto& vertex : vertices) {
            vertex.position = raw_mesh.positions[vertex_idx];
            vertex.normal = raw_mesh.normals[vertex_idx];
            vertex.uv = raw_mesh.uvs[vertex_idx];
            vertex_idx++;
        }

        this->vertices = vertices;
        this->indices = raw_mesh.indices;
        this->material = raw_mesh.material;

        init();
    }
}