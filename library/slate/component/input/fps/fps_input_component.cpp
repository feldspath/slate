#include "fps_input_component.hpp"

#include <slate/scene/object/slate_object.hpp>
#include <slate/input/input.hpp>

#include <iostream>

namespace slate {
    
    void FpsInputComponent::on_notify(Event event) {
        if (event.type != EventType::MOUSE_MOVE)
            return;

        glm::vec2 mouse_pos(event.read_float_arg("mouse_x"), event.read_float_arg("mouse_y"));
        glm::vec2 mouse_offset = mouse_pos - glm::vec2(mouse_last_x, mouse_last_y);
        mouse_last_x = mouse_pos.x;
        mouse_last_y = mouse_pos.y;


        if (first_mouse) {
            first_mouse = false;
            return;
        }

        auto obj = target.lock();
        if (!obj) {
            std::cerr << "Error::FpsInputComponent::Move: target expired\n";
            return;
        }

        auto camera_movement = glm::angleAxis(mouse_offset.x * sensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
        camera_movement *= glm::angleAxis(mouse_offset.y * sensitivity, obj->transform.right());

        obj->transform.rotation = camera_movement * obj->transform.rotation;
    }

    void FpsInputComponent::update() {
        auto obj = target.lock();
        if (!obj) {
            std::cerr << "Error::FpsInputComponent::Move: target expired\n";
            return;
        }

        glm::vec3 direction(0.0f, 0.0f, 0.0f);
        if (Input::get().key_pressed(GLFW_KEY_W)) direction += obj->transform.front();
        if (Input::get().key_pressed(GLFW_KEY_S)) direction -= obj->transform.front();
        if (Input::get().key_pressed(GLFW_KEY_D)) direction += obj->transform.right();
        if (Input::get().key_pressed(GLFW_KEY_A)) direction -= obj->transform.right();
        
        if (glm::dot(direction, direction) < 0.1f) return;

        obj->transform.position += glm::mat3(obj->transform.frame_matrix()) * glm::normalize(direction) * speed;
    }

}