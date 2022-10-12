#include "transform.hpp"

namespace slate {

    Transform::Transform() : position(0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f), scale(0.0f) {}

    glm::mat4 Transform::frame_matrix() const {
        // TODO: scale
        auto trans = glm::translate(glm::mat4(1.0f), position);
        auto rot = glm::toMat4(rotation);
        return trans * rot;
    }

    glm::vec3 Transform::front() const {
        return glm::mat3(frame_matrix()) * glm::vec3(0.0f, 0.0f, -1.0f);
    }

    glm::vec3 Transform::up() const {
        return glm::mat3(frame_matrix()) * glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::vec3 Transform::right() const {
        return glm::mat3(frame_matrix()) * glm::vec3(1.0f, 0.0f, 0.0f);
    }
}