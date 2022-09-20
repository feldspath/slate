#include "command.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace slate {
    CommandMove::CommandMove(glm::vec3 direction) {
        this->direction = direction;
    }

    void CommandMove::execute(ActorPtr actor) {
        actor->move(direction);
    }
}