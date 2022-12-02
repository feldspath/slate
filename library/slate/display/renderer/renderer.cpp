#include <string>
#include <iostream>

#include <slate/helper_dirs.hpp>
#include <slate/component/graphic/mesh_renderer/mesh_renderer_component.hpp>
#include <slate/input/input.hpp>
#include <slate/event/callbacks/callbacks.hpp>

#include "renderer.hpp"

#include <slate/utils/performance/performance.hpp>

#define MAX_LIGHTS 32

namespace slate {
    struct RenderMatrices {
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 view_inverse;
    };

    struct GPULight {
        glm::vec3 position;
        float padding;
        glm::vec3 color;
        float power;
    };

    Renderer::Renderer(unsigned int width, unsigned int height, const std::string& name) : window(std::make_shared<Window>(width, height, name.c_str())), fov(100.0f), near_plane(0.1f), far_plane(100.0f), ubo_matrices(3 * sizeof(glm::mat4), GL_DYNAMIC_DRAW), ubo_lights(MAX_LIGHTS * sizeof(GPULight) + sizeof(int), GL_DYNAMIC_DRAW) {
        load_shaders();
        window->disable_cursor();
        Callback::get().window_resize.add_observer(window);
        Input::get().set_window(window);
        glEnable(GL_DEPTH_TEST);

        // UBO
        ubo_matrices.bind(0);
        ubo_lights.bind(1);

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
        std::vector<GPULight> gpu_lights;
        int light_count = 0;
        for (auto& light : scene.get_lights()) {
            if (light->get_type() == LightType::POINT) {
                GPULight gpu_light = {
                    .position = light->transform.position,
                    .color = light->color,
                    .power = light->power
                };
                gpu_lights.push_back(gpu_light);
                ++light_count;
            }
            if (light_count == MAX_LIGHTS) break;
        }

        // Render
        ubo_matrices.update_buffer(0, sizeof(RenderMatrices), &render_matrices);
        ubo_lights.update_buffer(0, sizeof(GPULight) * light_count, gpu_lights.data());
        ubo_lights.update_buffer(sizeof(GPULight) * MAX_LIGHTS, sizeof(int), &light_count);

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