#pragma once

#include "slate/slate.hpp"
#include <slate/gpu_compute/compute_shader/compute_shader.hpp>
#include <slate/utils/performance/performance.hpp>
#include <slate/gpu_compute/ssbo/ssbo.hpp>

#include <vector>
#include <iostream>
#include <memory>

class CSTest : public slate::Component {
private:
    std::shared_ptr<slate::ComputeShader> cs;
    std::shared_ptr<slate::SSBO<float>> ssbo;

public:
    CSTest() {
        std::vector<float> data(1, 0.0f);
        ssbo = std::make_shared<slate::SSBO<float>>(data);
        cs = std::make_shared<slate::ComputeShader>(std::string(ROOT_DIR) + "resources/compute_shaders/test.glsl");
    }

    virtual void update(const float dt) override {
        slate::Benchmark bench("Compute Shader");
        cs->bind_ssbo(ssbo->get_id(), 0);
        cs->dispatch(1, 1, 1);
        std::vector<float> res(1);
        ssbo->read_data(0, 1, res);
        std::cout << res[0] << '\n';
    }
};