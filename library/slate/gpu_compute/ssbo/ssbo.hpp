#pragma once

#include <slate/display/glad/glad.hpp>

#include <memory>
#include <vector>

namespace slate {
    template <typename T>
    class SSBO {
    private:
        unsigned int id;
        size_t size;

    public:
        SSBO(const std::vector<T>& data);
        void read_data(unsigned int start, unsigned int end, std::vector<T>& data) const;
        unsigned int get_id() const;
    };

    template <typename T>
    SSBO<T>::SSBO(const std::vector<T>& data) {
        size = data.size() * sizeof(T);
        glGenBuffers(1, &id);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
        glBufferStorage(GL_SHADER_STORAGE_BUFFER, size, data.data(), GL_MAP_READ_BIT);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    template <typename T>
    void SSBO<T>::read_data(unsigned int start, unsigned int end, std::vector<T>& data) const {
        data.resize(end - start);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
        glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * start, data.size() * sizeof(T), data.data());
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    template <typename T>
    unsigned int SSBO<T>::get_id() const {
        return id;
    }
}