#pragma once

#include "../object/slate_object.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

namespace slate {
    enum class LightType {
        POINT,
        COUNT
    };

    class Light : public SlateObject {
    protected:
        LightType type;

    public:
        glm::vec3 color = glm::vec3(1.0f);    // in [0,1]^3
        float power = 20.0f;

        inline LightType get_type() const { return type; }
    };

    using LightPtr = std::shared_ptr<Light>;
}