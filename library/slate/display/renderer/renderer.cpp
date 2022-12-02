#include <string>
#include <iostream>

#include <slate/helper_dirs.hpp>
#include <slate/component/graphic/mesh_renderer/mesh_renderer_component.hpp>
#include <slate/input/input.hpp>
#include <slate/event/callbacks/callbacks.hpp>

#include "renderer.hpp"

#include <slate/utils/performance/performance.hpp>


namespace slate {
    struct RenderMatrices {
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 view_inverse;
    };

    Renderer::Renderer(unsigned int width, unsigned int height, const std::string& name) : window(std::make_shared<Window>(width, height, name.c_str())), fov(100.0f), near_plane(0.1f), far_plane(100.0f), ubo_matrices(3 * sizeof(glm::mat4), GL_DYNAMIC_DRAW) {
        load_shaders();
        window->disable_cursor();
        Callback::get().window_resize.add_observer(window);
        Input::get().set_window(window);
        glEnable(GL_DEPTH_TEST);

        // UBO
        ubo_matrices.bind(0);

        // GUI
        gui = std::make_shared<Gui>(window);
        Callback::get().chronos.add_observer(gui);
    }

    Renderer::~Renderer() {
    }

    void Renderer::run(Scene& scene) {
        auto& camera = scene.get_camera();
        if (!camera) {
            std::cerr << "Error::Renderer::run: the scene camera is not set\n";
            return;
        }
        float previous_time = glfwGetTime();
        while (should_continue()) {
            Benchmark bench("Main Loop");

            float current_time = glfwGetTime();
            scene.update(current_time-previous_time);
            previous_time = current_time;
            begin_frame();
            render(scene, scene.get_camera());
            end_frame();
        }
    }

    void Renderer::load_shaders() {
        std::string default_vs_path = std::string(SLATE_DIR) + std::string("display/shader/presets/default/default.vs");
        std::string default_fs_path = std::string(SLATE_DIR) + std::string("display/shader/presets/default/default.fs");
        default_shader = std::make_shared<slate::Shader>(default_vs_path, default_fs_path);
        default_shader->set_uniform_block("Matrices", 0);
    }

    void Renderer::render(const Scene& scene, const CameraPtr camera) {
        Benchmark bench("Rendering");

        // Matrices
        RenderMatrices render_matrices = {
            .projection = glm::perspective(glm::radians(fov), (float)window->get_width() / (float)window->get_height(), near_plane, far_plane),
            .view = camera->view_matrix(),
            .view_inverse = camera->transform.frame_matrix()
        };

        // Clear buffers
        glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Lights
        int light_count = 0;
        default_shader->use();
        for (auto& light : scene.get_lights()) {
            if (light->get_type() == LightType::POINT) {
                default_shader->set_uniform("lights[" + std::to_string(light_count) + "].position", light->transform.position);
                default_shader->set_uniform("lights[" + std::to_string(light_count) + "].color", light->color);
                default_shader->set_uniform("lights[" + std::to_string(light_count) + "].power", light->power);
                ++light_count;
            }
            if (light_count == 32) break;
        }
        default_shader->set_uniform("light_count", light_count);

        // Render
        ubo_matrices.update_buffer(0, sizeof(RenderMatrices), &render_matrices);
        auto render_objects = scene.components_by_type<GraphicComponent>();
        for (auto& o : render_objects) {
            o->render();
        }
    }

    void Renderer::begin_frame() {
        window->process_input();
        gui->init_frame();
    }

    void Renderer::end_frame() {
        gui->render();
        window->end_frame();
    }

    bool Renderer::should_continue() const {
        return !window->should_close();
    }

    ShaderPtr Renderer::get_default_shader() {
        return default_shader;
    }
}