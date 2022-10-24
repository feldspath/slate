#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <slate/scene/mesh/mesh.hpp>
#include <slate/scene/camera/camera_base.hpp>
#include <slate/scene/scene.hpp>
#include "../shader/shader.hpp"
#include "../window/window.hpp"


namespace slate {
    class Renderer {
    private:
        std::shared_ptr<Window> window;

        ShaderPtr default_shader;

        float fov;
        float near_plane;
        float far_plane;

        unsigned int ubo_matrices;

        void load_shaders();

    public:
        Renderer(unsigned int width, unsigned int height, const std::string& name);
        ~Renderer();

        glm::vec3 clear_color = glm::vec3(0.0f);

        void render(const Scene& scene, const CameraPtr camera);
        void begin_frame();
        void end_frame();
        bool should_continue() const;
        ShaderPtr get_default_shader();
    };
}