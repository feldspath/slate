#pragma once

#include "slate/slate.hpp"
#include <slate/gpu_compute/compute_shader/compute_shader.hpp>
#include <slate/utils/performance/performance.hpp>
#include <slate/gpu_compute/ssbo/ssbo.hpp>

#include <vector>
#include <iostream>
#include <memory>

#include <slate/gpu_compute/ssbo/ssbo.hpp>

class CSTest : public slate::Component {
private:
    std::shared_ptr<slate::ComputeShader> cs;
    std::shared_ptr<slate::SSBO> ssbo;

public:
    CSTest() {
        std::vector<float> data(1, 0.0f);
        ssbo = std::make_shared<slate::SSBO>(data.size() * sizeof(float), GL_MAP_READ_BIT, data.data());
        cs = std::make_shared<slate::ComputeShader>(std::string(ROOT_DIR) + "resources/compute_shaders/test.glsl");
    }

    virtual void update(const float dt) override {
        slate::Benchmark bench("Compute Shader");
        cs->bind_ssbo(ssbo->get_id(), 0);
        cs->dispatch(1, 1, 1);
        std::vector<float> res;
        ssbo->read_buffer(0, 1, res);
        std::cout << res[0] << '\n';
    }
};