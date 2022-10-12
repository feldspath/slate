#include "component.hpp"

#include <slate/scene/object/slate_object.hpp>

namespace slate {
    void Component::set_object(std::weak_ptr<SlateObject> target) {
        this->target_ptr = target;
    }

    std::shared_ptr<SlateObject> Component::target() {
        auto obj = target_ptr.lock();
        if (!obj) std::cerr << "Error::FpsInputComponent::Move: target expired\n";
        return obj;
    }
}