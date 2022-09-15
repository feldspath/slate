#pragma once

#include "../base/camera_base.hpp"
#include "slate/event/observer.hpp"


namespace slate {
    class CameraFPS : public CameraBase, public Observer {
    private:
        glm::vec2 mouse_last_pos;
        bool first_mouse;

    public:
        float sensitivity = 0.001f;

        CameraFPS();
        void on_notify(Event event) override;
    };
}