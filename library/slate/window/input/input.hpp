#pragma once

#include <GLFW/glfw3.h>

#include <slate/camera/base/camera_base.hpp>

#include "command.hpp"

namespace slate {
    class InputHandler {
    private:
        CommandPtr key_w;
        CommandPtr key_s;
        CommandPtr key_a;
        CommandPtr key_d;
        ActorPtr camera;

    public:
        InputHandler();
        void handle_input(GLFWwindow* window);
        void set_camera_ptr(CameraPtr camera);
    };
}