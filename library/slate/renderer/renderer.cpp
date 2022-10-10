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
    }

    Renderer::~Renderer() {

    }

    void Renderer::load_shaders() {
        std::string default_vs_path = std::string(SLATE_DIR) + std::string("shader/presets/default/default.vs");
        std::string default_fs_path = std::string(SLATE_DIR) + std::string("shader/presets/default/default.fs");
        default_shader = std::make_shared<slate::Shader>(default_vs_path, default_fs_path);
    }

    void Renderer::render(Scene scene) {
        // Matrices
        const glm::mat4 view_matrix = camera->view_matrix();
        const glm::mat4 projection_matrix = glm::perspective(glm::radians(fov), (float)window->get_width() / (float)window->get_height(), near_plane, far_plane);

        // Clear buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render
        default_shader->use();
        default_shader->set_uniform("view_matrix", view_matrix);
        default_shader->set_uniform("projection_matrix", projection_matrix);
        auto instance = scene.slate_object_by_name("triangle");
        auto render_comp = instance->get_component<MeshRendererComponent>();
        render_comp->render();

        // for (const auto& instance : scene.get_map()) {
        //     default_shader->set_uniform("model_matrix", instance.second->get_transform());
        //     instance.second->draw();
        // }
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