#pragma once

#include "slate/slate.hpp"
#include <slate/gpu_compute/compute_shader/compute_shader.hpp>

#include <vector>
#include <iostream>

class CSTest : public slate::Component {
private:
    slate::ComputeShader cs;

public:
    CSTest() {
        std::vector<float> data(1, 0.0f);
        cs = slate::ComputeShader(std::string(ROOT_DIR) + "compute_shaders/test.glsl", data);
    }

    virtual void update(const float dt) override {
        cs.dispatch(1, 1, 1);
        std::vector<float> res;
        cs.read_data(0, 1, res);
        std::cout << res[0] << '\n';
    }
};