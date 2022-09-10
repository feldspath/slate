#include "renderer.hpp"

#include "slate/window/window.hpp"
#include "slate/helper_dirs.hpp"

#include <string>

namespace slate {
    Renderer::Renderer(unsigned int width, unsigned int height, const std::string& name) : window(width, height, name.c_str()) {
        load_shaders();
    }

    Renderer::~Renderer() {
        
    }

    void Renderer::load_shaders() {
        std::string default_vs_path = std::string(SLATE_DIR) + std::string("shader/presets/default/default.vs");
        std::string default_fs_path = std::string(SLATE_DIR) + std::string("shader/presets/default/default.fs");
        default_shader = std::make_shared<slate::Shader>(default_vs_path, default_fs_path);
    }

    void Renderer::render() {
        // Matrices
        const auto view_matrix = glm::mat4(1.0f);
        const auto projection_matrix = glm::perspective(glm::radians(45.0f), (float)window.get_width()/(float)window.get_height(), 0.1f, 100.0f);
        
        // Clear buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render
        default_shader->use();
        default_shader->set_uniform("view_matrix", view_matrix);
        default_shader->set_uniform("projection_matrix", projection_matrix);
        for (const auto& instance : instances) {
            default_shader->set_uniform("model_matrix", instance.get_transform());
            instance.draw();
        }
    }

    void Renderer::add_instance(MeshPtr mesh, glm::mat4 transform) {
        meshes.push_back(mesh);
        instances.emplace_back(mesh, transform);
    }

    void Renderer::begin_frame() {
        window.process_input();
    }

    void Renderer::end_frame() {
        window.end_frame();
    }

    bool Renderer::should_continue() const {
        return !window.should_close(); 
    }


}