#include "component.hpp"

#include <slate/scene/object/slate_object.hpp>

namespace slate {
    void Component::set_object(std::weak_ptr<SlateObject> target) {
        this->target = target;
    }
}