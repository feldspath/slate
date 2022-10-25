#include "input.hpp"

#include <iostream>

namespace slate {
    Input& Input::get() {
        static Input input;
        return input;
    }
    
    bool Input::key_pressed(unsigned int key) const {
        auto w = window.lock();
        if (!w) {
            std::cout << "Error::Input::key_pressed: window not set or expired\n";
        }
        return w->key_pressed(key);
    }

    void Input::set_window(std::weak_ptr<Window> window) {
        this->window = window;
    }

}