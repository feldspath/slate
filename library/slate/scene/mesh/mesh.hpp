#pragma once

#include <memory>
#include <vector>

#include <slate/glad/glad.hpp>
#include <slate/shader/shader.hpp>
#include "../material/material.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace slate {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    class Mesh {
    private:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::weak_ptr<Material> material;

        unsigned int vao, vbo, ebo;
        unsigned int n_triangles;

    public:
        Mesh(const std::vector<Vertex> vertices_, const std::vector<unsigned int> indices_, const std::weak_ptr<Material> material_);
        ~Mesh();

        void draw(const ShaderPtr shader) const;
    };

    using MeshPtr = std::shared_ptr<Mesh>;
}