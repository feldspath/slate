#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <slate/scene/mesh/mesh.hpp>
#include <slate/scene/camera/camera_base.hpp>
#include <slate/scene/scene.hpp>
#include <slate/gpu_compute/ubo/ubo.hpp>

#include "../shader/shader.hpp"
#include "../window/window.hpp"
#include "../gui/gui.hpp"


namespace slate {
    class Renderer {
    private:
        std::shared_ptr<Window> window;
        std::shared_ptr<Gui> gui;

        ShaderPtr default_shader;

        float fov;
        float near_plane;
        float far_plane;

        UBO ubo_matrices;
        UBO ubo_lights;

        void load_shaders();
        void render(std::shared_ptr<Scene> scene, const CameraPtr camera);
        void begin_frame();
        void end_frame();
        bool should_continue() const;

    public:
        Renderer(unsigned int width, unsigned int height, const std::string& name);
        ~Renderer();

        void run(std::shared_ptr<Scene> scene);
        glm::vec3 clear_color = glm::vec3(0.0f);
        ShaderPtr get_default_shader();
    };
}