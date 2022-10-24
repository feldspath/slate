#pragma once

#include "../mesh/mesh.hpp"
#include "../material/material.hpp"
#include <slate/display/shader/shader.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

namespace slate {
    class Model {
    private:
        std::vector<MeshPtr> meshes;
        std::vector<std::shared_ptr<Material>> materials;

        void load_model(const std::string path);
        void process_node(aiNode* node, const aiScene* scene);
        MeshPtr process_mesh(aiMesh* mesh, const aiScene* scene);

    public:
        Model(const std::string path);
        void draw(const ShaderPtr shader) const;
    };

    using ModelPtr = std::shared_ptr<Model>;
}