#pragma once

#include "slate/slate.hpp"
#include <slate/gpu_compute/compute_shader/compute_shader.hpp>
#include <slate/utils/performance/performance.hpp>

#include <vector>
#include <iostream>
#include <memory>

class CSTest : public slate::Component {
private:
    std::unique_ptr<slate::ComputeShader> cs;

public:
    CSTest() {
        std::vector<float> data(1, 0.0f);
        cs = std::make_unique<slate::ComputeShader>(std::string(ROOT_DIR) + "compute_shaders/test.glsl", data);
    }

    virtual void update(const float dt) override {
        slate::Benchmark bench("Compute Shader");
        cs->dispatch(1, 1, 1);
        //std::vector<float> res(1);
        //cs->read_data(0, 1, res);
        //std::cout << res[0] << '\n';
    }
};