#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace slate {
    class Actor {
    public:
        virtual void move(glm::vec3 direction) = 0;
    };

    using ActorPtr = std::shared_ptr<Actor>;
}