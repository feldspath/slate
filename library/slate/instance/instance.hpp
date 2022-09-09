#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../mesh/mesh.hpp"
#include "../shader/shader.hpp"

namespace slate {
    class Instance {
    private:
        MeshPtr mesh;
        glm::mat4 transform;

    public:
        Instance(MeshPtr mesh, glm::mat4 transform_ = glm::mat4(1.0f));

        void draw() const;
        glm::mat4& get_transform();
        const glm::mat4& get_transform() const;
    };
}