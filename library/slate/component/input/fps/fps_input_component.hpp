#pragma once

#include <slate/component/component.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace slate {
    class FpsInputComponent : public Component {
    private:
        float mouse_last_x;
        float mouse_last_y;
        bool first_mouse = true;

    public:
        float sensitivity = 0.001f;
        float speed = 0.1f;
        void on_notify(Event event) override;    
        void update(const float dt) override;
    };
}