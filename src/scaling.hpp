#pragma once

#include "slate/slate.hpp"

class Scaling : public slate::Component {
private:
    float time = 0.0f;

public:
    virtual void update(const float dt) override {
        time += dt;
        auto targ = get_target();
        targ->transform.scale = glm::vec3(0.5f + 0.4 * std::cos(time));
    }
};