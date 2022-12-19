#pragma once

#include <memory>
#include <vector>
#include <iostream>

#include <slate/display/glad/glad.hpp>
#include <slate/display/shader/shader.hpp>
#include <slate/utils/model_loader/model_loader.hpp>
#include "../material/material.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace slate {
    struct ParamMetadata {
        GLint size;
        GLenum type;
        void* offset;
    };

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    template<typename T>
    class Mesh {
    private:
        std::vector<T> vertices;
        std::vector<unsigned int> indices;
        std::shared_ptr<Material> material;

        unsigned int vao, vbo, ebo;
        unsigned int n_triangles;

        void init(std::vector<ParamMetadata> params_info = std::vector<ParamMetadata>());

    public:
        Mesh(const std::string& path);
        Mesh(const std::vector<T> vertices_, const std::vector<unsigned int> indices_, const std::shared_ptr<Material> material_, std::vector<ParamMetadata> params_info = std::vector<ParamMetadata>());
        ~Mesh();

        void draw(const ShaderPtr shader) const;
    };

    using MeshPtr = std::shared_ptr<Mesh<Vertex>>;

    template<typename T> Mesh<T>::Mesh(const std::vector<T> vertices_, const std::vector<unsigned int> indices_, const std::shared_ptr<Material> material_, std::vector<ParamMetadata> params_info) : vertices(vertices_), indices(indices_), material(material_) {
        init(params_info);
    }

    template<typename T> Mesh<T>::~Mesh() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    template<typename T> void Mesh<T>::init(std::vector<ParamMetadata> params_info) {
        assert(vertices.size() > 0);
        unsigned int n_indices = indices.size();
        n_triangles = n_indices / 3;

        if (n_indices % 3 != 0) {
            std::cerr << "Warning::Mesh: incorrect number of indices\n";
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(T), (void*)offsetof(T, position));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(T), (void*)offsetof(T, normal));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(T), (void*)offsetof(T, uv));
        glEnableVertexAttribArray(2);

        int idx = 3;
        for (const ParamMetadata& p : params_info) {
            switch (p.type) {
            case GL_FLOAT:
                glVertexAttribPointer(idx, p.size, p.type, GL_FALSE, sizeof(T), p.offset);
                break;
            case GL_UNSIGNED_INT:
                glVertexAttribIPointer(idx, p.size, p.type, sizeof(T), p.offset);
                break;
            case GL_INT:
                glVertexAttribIPointer(idx, p.size, p.type, sizeof(T), p.offset);
                break;
            default:
                std::cerr << "Error::Mesh: additional parameter type not supported\n";
                continue;
            }

            glEnableVertexAttribArray(idx);
            ++idx;
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    template<typename T> void Mesh<T>::draw(const ShaderPtr shader) const {
        // assume the shader is already used, we only pass the shader for uniforms
        if (!material) {
            std::cerr << "Error::Mesh::Draw: material has expired\n";
            abort();
        }

        shader->set_uniform("diffuse", material->diffuse);
        shader->set_uniform("ambient", material->ambient);
        shader->set_uniform("specular", material->specular);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, n_triangles * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}