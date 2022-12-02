#include "ubo.hpp"

namespace slate {
    UBO::UBO(size_t size, GLenum usage, void* data) : GPUBuffer(GL_UNIFORM_BUFFER, size) {
        glBindBuffer(type, id);
        glBufferData(type, size, data, usage);
        glBindBuffer(type, 0);

    }

    void UBO::bind(unsigned int index) const {
        glBindBufferRange(type, index, id, 0, size);
    }
}