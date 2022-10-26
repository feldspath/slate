#pragma once

#include "../component.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace slate {
    class Transform : public Component {
    public:
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        Transform();

        void update(const float) override {}

        glm::mat4 frame_matrix() const;
        glm::vec3 front() const;
        glm::vec3 up() const;
        glm::vec3 right() const;
    };
}