#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <memory>

namespace slate {
    class CameraBase {
    protected:
        glm::vec3 position;
        glm::quat orientation;

    public:
        CameraBase();
        CameraBase(glm::vec3 position_, glm::quat orientation_);
        glm::mat4 frame_matrix() const;
        glm::mat4 view_matrix() const;

        glm::vec3 front() const;
        glm::vec3 up() const;
        glm::vec3 right() const;
    };

    using CameraPtr = std::shared_ptr<CameraBase>;
}