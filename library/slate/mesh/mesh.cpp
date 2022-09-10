#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.hpp"


namespace slate {
    Mesh::Mesh(const std::vector<glm::vec3> vertices) {
        n_vertices = vertices.size();
        n_triangles = n_vertices / 3;

        if (n_vertices % 3 != 0) {
            std::cerr << "Warning::Mesh: incorrect number of vertices\n";
        }
        
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0); 
    }

    Mesh::~Mesh() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    void Mesh::draw() {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, n_triangles * 3);
        glBindVertexArray(0);
    }
}