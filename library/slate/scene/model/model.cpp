#include "model.hpp"

namespace slate {
    Model::Model(const std::vector<RawMesh>& raw_meshes, const std::vector<std::shared_ptr<Material>>& materials) : materials(materials) {
        init(raw_meshes);
    }

    Model::Model(const std::string& path) {
        ModelLoader loader(path);
        materials = loader.get_materials();
        init(loader.get_meshes());
    }

    void Model::init(const std::vector<RawMesh>& raw_meshes) {
        meshes.resize(raw_meshes.size());

        int mesh_idx = 0;
        for (const auto& raw_mesh : raw_meshes) {
            std::vector<Vertex> vertices(raw_mesh.positions.size());
            int vertex_idx = 0;
            for (auto& vertex : vertices) {
                vertex.position = raw_mesh.positions[vertex_idx];
                vertex.normal = raw_mesh.normals[vertex_idx];
                vertex.uv = raw_mesh.uvs[vertex_idx];
                vertex_idx++;
            }

            meshes[mesh_idx++] = std::make_shared<Mesh<Vertex>>(vertices, raw_mesh.indices, raw_mesh.material);
        }
    }

    void Model::draw(const ShaderPtr shader) const {
        for (const auto& m : meshes) {
            m->draw(shader);
        }
    }
}