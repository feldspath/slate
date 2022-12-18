#pragma once

#include <slate/display/glad/glad.hpp>

#include <vector>

namespace slate {

    class GPUBuffer {
    protected:
        unsigned int id;
        GLenum type;
        size_t size;

    public:
        GPUBuffer(GLenum type, size_t size);
        ~GPUBuffer();

        inline unsigned int get_id() const;

        void update_buffer(size_t offset, size_t update_size, void* data);
        void read_buffer(size_t offset, size_t read_size, void* data) const;

        template <typename T>
        void update_buffer(size_t element_offset, std::vector<T>& data);

        template <typename T>
        void read_buffer(size_t element_offset, size_t n_elements, std::vector<T>& data) const;
    };

    inline unsigned int GPUBuffer::get_id() const {
        return id;
    }

    template <typename T>
    void GPUBuffer::read_buffer(size_t element_offset, size_t n_elements, std::vector<T>& data) const {
        data.resize(n_elements);
        read_buffer(element_offset * sizeof(T), n_elements * sizeof(T), data.data());
    }

    template <typename T>
    void GPUBuffer::update_buffer(size_t element_offset, std::vector<T>& data) {
        update_buffer(element_offset * sizeof(T), data.size() * sizeof(T), data.data());
    }
}