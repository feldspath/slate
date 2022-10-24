#include "camera_base.hpp"

namespace slate {
    Camera::Camera() {}

    glm::mat4 Camera::view_matrix() const {
        return glm::inverse(transform.frame_matrix());
    }
}