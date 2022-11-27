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

        bool show_demo_window = true;
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
    }

    void Gui::render() {
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window->get_window(), &display_w, &display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


}