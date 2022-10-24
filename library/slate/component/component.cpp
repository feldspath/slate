#include "component.hpp"

#include <slate/scene/object/slate_object.hpp>

namespace slate {
    void Component::set_object(std::weak_ptr<SlateObject> get_target) {
        this->target_ptr = get_target;
    }

    std::shared_ptr<SlateObject> Component::get_target() {
        auto obj = target_ptr.lock();
        if (!obj) std::cerr << "Error::FpsInputComponent::Move: target expired\n";
        return obj;
    }
}