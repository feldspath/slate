#include "camera_base.hpp"

namespace slate {
    CameraBase::CameraBase() {}

    glm::mat4 CameraBase::view_matrix() const {
        return glm::inverse(transform.frame_matrix());
    }
}