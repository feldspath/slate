#pragma once

#include <slate/actor/actor.hpp>
#include <memory>



namespace slate {
    class Command {

    public:
        virtual void execute(ActorPtr actor) = 0;
    };

    using CommandPtr = std::shared_ptr<Command>;

    class CommandMove : public Command {
    public:
        glm::vec3 direction;
        CommandMove(glm::vec3 direction);
        void execute(ActorPtr actor) override;
    };
}