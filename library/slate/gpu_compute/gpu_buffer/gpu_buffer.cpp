#include "gpu_buffer.hpp"
#include <assert.h>

namespace slate {
    GPUBuffer::GPUBuffer(GLenum type, size_t size) : type(type), size(size) {
        glGenBuffers(1, &id);
    }

    GPUBuffer::~GPUBuffer() {
        glDeleteBuffers(1, &id);
    }


    void GPUBuffer::update_buffer(size_t offset, size_t update_size, void* data) {
        assert(update_size + offset <= size);
        glBindBuffer(type, id);
        glBufferSubData(type, offset, update_size, data);
        glBindBuffer(type, 0);
    }

    void GPUBuffer::read_buffer(size_t offset, size_t read_size, void* data) const {
        assert(read_size + offset <= size);
        glBindBuffer(type, id);
        glGetBufferSubData(type, offset, read_size, data);
        glBindBuffer(type, 0);
    }
}