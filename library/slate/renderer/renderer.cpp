#include <string>
#include <iostream>

#include "slate/window/window.hpp"
#include "slate/helper_dirs.hpp"
#include "slate/component/graphic/mesh_renderer/mesh_renderer_component.hpp"

#include "renderer.hpp"

#include "../window/callbacks.hpp"


namespace slate {
    Renderer::Renderer(unsigned int width, unsigned int height, const std::string& name) : window(std::make_shared<Window>(width, height, name.c_str())), camera(std::make_shared<CameraBase>()), fov(100.0f), near_plane(0.1f), far_plane(100.0f) {
        load_shaders();
        // window->disable_cursor();
        Callback::get().window_resize.add_observer(window);
        // Callback::get().mouse_move.add_observer(camera);
        // window->input_handler.set_camera_ptr(camera);

        // UBO
        glGenBuffers(1, &ubo_matrices);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo_matrices);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo_matrices, 0, 2 * sizeof(glm::mat4));
    }

    Renderer::~Renderer() {
        glDeleteBuffers(1, &ubo_matrices);
    }

    void Renderer::load_shaders() {
        std::string default_vs_path = std::string(SLATE_DIR) + std::string("shader/presets/default/default.vs");
        std::string default_fs_path = std::string(SLATE_DIR) + std::string("shader/presets/default/default.fs");
        default_shader = std::make_shared<slate::Shader>(default_vs_path, default_fs_path);
        default_shader->set_uniform_block("Matrices", 0);
    }

    void Renderer::render(Scene scene) {
        // Matrices
        const glm::mat4 view_matrix = camera->view_matrix();
        const glm::mat4 projection_matrix = glm::perspective(glm::radians(fov), (float)window->get_width() / (float)window->get_height(), near_plane, far_plane);

        // Clear buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render
        glBindBuffer(GL_UNIFORM_BUFFER, ubo_matrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection_matrix[0][0]);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view_matrix[0][0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        auto render_objects = scene.components_by_type<GraphicComponent>();
        for (auto& o : render_objects) {
            o->render();
        }
    }

    void Renderer::begin_frame() {
        window->process_input();
    }

    void Renderer::end_frame() {
        window->end_frame();
    }

    bool Renderer::should_continue() const {
        return !window->should_close();
    }

    ShaderPtr Renderer::get_default_shader() {
        return default_shader;
    }


}