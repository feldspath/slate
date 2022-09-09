#pragma once

#include <vector>

#include "../mesh/mesh.hpp"
#include "../shader/shader.hpp"
#include "../instance/instance.hpp"
#include "../window/window.hpp"

namespace slate {
    class Renderer {
    private:
        Window window;
        std::vector<MeshPtr> meshes;
        std::vector<Instance> instances;
        ShaderPtr default_shader;

        void load_shaders();

    public:
        Renderer(unsigned int width, unsigned int height, const std::string& name);
        ~Renderer();

        void render();
        void add_instance(MeshPtr mesh, glm::mat4 transform = glm::mat4(1.0f));
        void begin_frame();
        void end_frame();
        bool should_continue() const;
    };
}