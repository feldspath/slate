#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <slate/scene/object/slate_object.hpp>

#include <memory>

namespace slate {
    class Camera : public SlateObject {
    public:
        Camera();
        glm::mat4 view_matrix() const;
    };

    using CameraPtr = std::shared_ptr<Camera>;
}