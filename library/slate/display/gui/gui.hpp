#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <slate/display/window/window.hpp>
#include <slate/event/observer.hpp>

#include <memory>
#include <map>

namespace slate {
    class Gui : public Observer {
    private:

        std::shared_ptr<Window> window;

        std::map<std::string, float> benches;

    public:
        Gui(std::shared_ptr<Window> window);
        ~Gui();
        void init_frame();
        void render();
        void on_notify(Event e) override;
    };
}