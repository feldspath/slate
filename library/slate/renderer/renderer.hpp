#pragma once

#include <vector>

#include "slate/mesh/mesh.hpp"
#include "slate/shader/shader.hpp"
#include "slate/instance/instance.hpp"
#include "slate/window/window.hpp"
#include "slate/scene/scene.hpp"

namespace slate {
    class Renderer {
    private:
        Window window;
        ShaderPtr default_shader;

        void load_shaders();

    public:
        Renderer(unsigned int width, unsigned int height, const std::string& name);
        ~Renderer();

        void render(Scene scene);
        void begin_frame();
        void end_frame();
        bool should_continue() const;
    };
}