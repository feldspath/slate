#pragma once

#include "../input_component.hpp"
#include <slate/event/observer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace slate {
    class FpsInputComponent : public InputComponent, public Observer {
    private:
        float mouse_last_x;
        float mouse_last_y;
        bool first_mouse = false;    

    public:
        float sensitivity = 0.001f;
        float speed = 0.1f;
        void on_notify(Event event) override;    
        void update() override;
    };
}