#pragma once

#include <slate/scene/material/material.hpp>
#include <slate/scene/mesh/mesh.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <memory>

namespace slate {
    struct RawMesh {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        std::vector<unsigned int> indices;
        std::weak_ptr<Material> material;
    };

    class ModelLoader {
    private:
        std::vector<RawMesh> meshes;
        std::vector<std::shared_ptr<Material>> materials;

        void process_node(aiNode* node, const aiScene* scene);
        RawMesh process_mesh(aiMesh* mesh, const aiScene* scene);
    public:
        void load_model(const std::string path);
        std::vector<RawMesh> get_meshes() const;
        std::vector<std::shared_ptr<Material>> get_materials() const;
    };
}