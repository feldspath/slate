#pragma once

#include "../gpu_buffer/gpu_buffer.hpp"

namespace slate {
    class UBO : public GPUBuffer {
    private:    

    public:
        UBO(size_t size, GLenum usage, void* data = nullptr);

        void bind(unsigned int index) const;
    };
}