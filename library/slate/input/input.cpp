#include "input.hpp"

#include <iostream>

namespace slate {
    Input& Input::get() {
        static Input input;
        return input;
    }
    
    bool Input::key_pressed(unsigned int key) const {
        if (!window) {
            std::cout << "Error::Input::key_pressed: window not set\n";
        }
        return window->key_pressed(key);
    }

    void Input::set_window(std::shared_ptr<Window> window) {
        this->window = window;
    }

}