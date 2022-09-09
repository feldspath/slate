#include "renderer.hpp"

namespace slate {
    Renderer::Renderer() {
        load_shaders();
    }

    Renderer::~Renderer() {

    }

    void Renderer::load_shaders() {
        default_shader = std::make_shared<slate::Shader>("src/shaders/default.vs", "src/shaders/default.fs");
    }

    void Renderer::render() {
        default_shader->use();
        for (const auto& instance : instances) {
            instance.draw();
        }
    }

    void Renderer::add_instance(MeshPtr mesh, glm::mat4 transform) {
        meshes.push_back(mesh);
        instances.emplace_back(mesh, transform);
    }


}