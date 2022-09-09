#pragma once

#include "../glad/glad.hpp"
#include <GLFW/glfw3.h>

#include <string>

namespace slate {
    class Window {
    private:
        GLFWwindow* id;
        void process_input();

    public:
        Window(unsigned int width, unsigned int height, std::string name);
        ~Window();

        void begin_frame();
        void end_frame();
        bool should_continue();

    };
}
