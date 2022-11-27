#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <slate/display/window/window.hpp>

#include <memory>

namespace slate {
    class Gui {
    private:

        std::shared_ptr<Window> window;

    public:
        Gui(std::shared_ptr<Window> window);
        ~Gui();
        void init_frame();
        void render();

    };
}