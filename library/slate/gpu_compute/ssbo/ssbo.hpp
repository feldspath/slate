#pragma once

#include <slate/display/glad/glad.hpp>
#include "../gpu_buffer/gpu_buffer.hpp"

#include <memory>
#include <vector>

namespace slate {
    class SSBO : public GPUBuffer {
    public:
        SSBO(size_t size, GLenum usage, void* data);
    };
}