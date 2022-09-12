#include "instance.hpp"

namespace slate {
    Instance::Instance(MeshPtr mesh_, glm::mat4 transform_) : mesh(mesh_), transform(transform_) {}

    void Instance::draw() const {
        mesh->draw();
    }

    glm::mat4& Instance::get_transform() {
        return transform;
    }

    const glm::mat4& Instance::get_transform() const {
        return transform;
    }
}