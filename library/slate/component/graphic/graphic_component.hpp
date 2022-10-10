#pragma once

#include "../component.hpp"

namespace slate {
    class GraphicComponent : public Component {
    public:
        virtual void render() = 0;
    };
}