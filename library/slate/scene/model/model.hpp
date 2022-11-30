#pragma once

#include "../mesh/mesh.hpp"
#include "../material/material.hpp"
#include <slate/display/shader/shader.hpp>
#include <slate/utils/model_loader/model_loader.hpp>

#include <vector>
#include <string>
#include <memory>

namespace slate {
    class Model {
    private:
        std::vector<MeshPtr> meshes;
        std::vector<std::shared_ptr<Material>> materials;

        void init(const std::vector<RawMesh>& raw_meshes);

    public:
        Model(const std::string& path);
        Model(const std::vector<RawMesh>& meshes, const std::vector<std::shared_ptr<Material>>& materials);
        void draw(const ShaderPtr shader) const;
    };

    using ModelPtr = std::shared_ptr<Model>;
}