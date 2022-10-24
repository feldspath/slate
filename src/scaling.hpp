#pragma once

#include "slate/slate.hpp"

class Scaling : public slate::Component {

    virtual void update() override {
        auto targ = get_target();
        targ->transform.scale = glm::vec3(0.5f + 0.4 * std::cos(glfwGetTime()));
    }
};