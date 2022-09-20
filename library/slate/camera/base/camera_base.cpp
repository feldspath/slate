#include "camera_base.hpp"

namespace slate {
    CameraBase::CameraBase() : position(0.0f, 0.0f, 0.0f), orientation(1.0f, 0.0f, 0.0f, 0.0f) {}

    CameraBase::CameraBase(glm::vec3 position_, glm::quat orientation_) : position(position_), orientation(orientation_) {}

    glm::mat4 CameraBase::frame_matrix() const {
        auto view_matrix = glm::toMat4(orientation);
        view_matrix = glm::translate(view_matrix, position);
        return view_matrix;
    }

    glm::mat4 CameraBase::view_matrix() const {
        return glm::inverse(frame_matrix());
    }

    glm::vec3 CameraBase::front() const {
        return glm::mat3(frame_matrix()) * glm::vec3(0.0f, 0.0f, -1.0f);
    }

    glm::vec3 CameraBase::up() const {
        return glm::mat3(frame_matrix()) * glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::vec3 CameraBase::right() const {
        return glm::mat3(frame_matrix()) * glm::vec3(1.0f, 0.0f, 0.0f);
    }
}