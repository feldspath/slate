#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.hpp"


namespace slate {
    Mesh::Mesh(const std::vector<Vertex> vertices_, const std::vector<unsigned int> indices_, const std::weak_ptr<Material> material_) : vertices(vertices_), indices(indices_), material(material_) {
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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    Mesh::~Mesh() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    void Mesh::draw(const ShaderPtr shader) const {
        // assume the shader is already used, we only pass the shader for uniforms
        auto mat = material.lock();
        if (!mat) {
            std::cerr << "Error::Mesh::Draw: material has expired\n";
        }

        shader->set_uniform("diffuse", mat->diffuse);
        shader->set_uniform("ambient", mat->ambient);
        shader->set_uniform("specular", mat->specular);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, n_triangles * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}