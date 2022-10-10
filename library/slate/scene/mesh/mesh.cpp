#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.hpp"


namespace slate {
    Mesh::Mesh(const std::vector<glm::vec3> vertices, const std::vector<unsigned int> indices) {
        n_vertices = vertices.size();
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
        glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    Mesh::~Mesh() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    void Mesh::draw() {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, n_triangles * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}