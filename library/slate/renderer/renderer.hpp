#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "slate/scene/mesh/mesh.hpp"
#include "slate/shader/shader.hpp"
#include "slate/window/window.hpp"
#include "slate/scene/scene.hpp"
#include "slate/scene/camera/base/camera_base.hpp"


namespace slate {
    class Renderer {
    private:
        std::shared_ptr<Window> window;
        std::shared_ptr<CameraBase> camera;

        ShaderPtr default_shader;
        glm::mat4 projection;

        float fov;
        float near_plane;
        float far_plane;

        void load_shaders();

    public:
        Renderer(unsigned int width, unsigned int height, const std::string& name);
        ~Renderer();

        void render(Scene scene);
        void begin_frame();
        void end_frame();
        bool should_continue() const;
        ShaderPtr get_default_shader();
    };
}