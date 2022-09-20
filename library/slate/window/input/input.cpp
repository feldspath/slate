#include "input.hpp"

#include <iostream>

namespace slate {
    InputHandler::InputHandler() {
        key_w = std::make_shared<CommandMove>(glm::vec3(0.0f, 0.0f, -1.0f));
        key_s = std::make_shared<CommandMove>(glm::vec3(0.0f, 0.0f, 1.0f));
        key_d = std::make_shared<CommandMove>(glm::vec3(1.0f, 0.0f, 0.0f));
        key_a = std::make_shared<CommandMove>(glm::vec3(-1.0f, 0.0f, 0.0f));
    }


    void InputHandler::set_camera_ptr(CameraPtr camera) {
        this->camera = std::shared_ptr<Actor>(camera.get());
    }

    void InputHandler::handle_input(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            if (!camera) return;
            key_w->execute(camera);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            if (!camera) return;
            key_s->execute(camera);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            if (!camera) return;
            key_a->execute(camera);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            if (!camera) return;
            key_d->execute(camera);
        }
    }
}