#pragma once

#include <vector>

#include "../mesh/mesh.hpp"
#include "../shader/shader.hpp"
#include "../instance/instance.hpp"

namespace slate {
    class Renderer {
    private:
        std::vector<MeshPtr> meshes;
        std::vector<Instance> instances;

        ShaderPtr default_shader;

        void load_shaders();

    public:
        Renderer();
        ~Renderer();

        void render();
        void add_instance(MeshPtr mesh, glm::mat4 transform = glm::mat4(1.0f));
    };
}