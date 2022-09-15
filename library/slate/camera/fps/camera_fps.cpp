#include <memory>

#include "camera_fps.hpp"
#include "slate/window/callbacks.hpp"


namespace slate {
    CameraFPS::CameraFPS() : CameraBase(), mouse_last_pos(0.0f, 0.0f), first_mouse(true) {
    }

    void CameraFPS::on_notify(Event event) {
        if (event.type != EventType::MOUSE_MOVE)
            return;

        glm::vec2 mouse_pos(event.read_float_arg("mouse_x"), event.read_float_arg("mouse_y"));
        glm::vec2 mouse_offset = mouse_pos - mouse_last_pos;
        mouse_last_pos = mouse_pos;

        if (first_mouse) {
            first_mouse = false;
            return;
        }

        auto camera_movement = glm::angleAxis(mouse_offset.x * sensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
        camera_movement *= glm::angleAxis(mouse_offset.y * sensitivity, right());

        orientation = camera_movement * orientation;
    }
}