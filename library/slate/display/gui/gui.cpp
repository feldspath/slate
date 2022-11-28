#include "gui.hpp"

namespace slate {
    Gui::Gui(std::shared_ptr<Window> window) : window(window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForOpenGL(window->get_window(), true);
        ImGui_ImplOpenGL3_Init("#version 460");
        ImGui::StyleColorsDark();
    }

    Gui::~Gui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Gui::init_frame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::CollapsingHeader("Benchmarks", ImGuiTreeNodeFlags_DefaultOpen)) {
            for (auto &b : benches) {
                std::string t = b.first + ": " + std::to_string(b.second) + "ms";
                ImGui::Text(t.c_str());
            }
        }
    }

    void Gui::render() {
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window->get_window(), &display_w, &display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Gui::on_notify(Event e) {
        if (e.type != EventType::CHRONO_DONE)
            return;

        std::string name = (*e.arguments.cbegin()).first;
        float time = e.read_float_arg(name);
        benches.insert_or_assign(name, time);
    }


}