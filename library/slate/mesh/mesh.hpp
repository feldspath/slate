#pragma once

#include "../glad/glad.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <vector>

namespace slate {
    class Mesh {
    private:
        unsigned int vao, vbo, ebo;
        unsigned int n_vertices;
        unsigned int n_triangles;

    public:
        Mesh(const std::vector<glm::vec3> vertices);
        ~Mesh();

        void draw();
    };

    using MeshPtr = std::shared_ptr<Mesh>;
}