#include "ssbo.hpp"

#include <iostream>

namespace slate {
    SSBO::SSBO(size_t size, GLenum usage, void* data) : GPUBuffer(GL_SHADER_STORAGE_BUFFER, size) {
        glBindBuffer(type, id);
        glBufferStorage(type, size, data, usage);
        glBindBuffer(type, 0);
    }
}