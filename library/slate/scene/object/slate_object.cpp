#include "slate_object.hpp"

namespace slate {
    unsigned int SlateObject::object_count = 0;

    SlateObject::SlateObject(std::string name_) : id(object_count++), name(name_) {}

    SlateObject::SlateObject() : SlateObject(std::string("SlateObject") + std::to_string(object_count)) {}

    const std::string& SlateObject::get_name() {
        return name;
    }

    void SlateObject::add_component(ComponentPtr component) {
        components.push_back(component);
        component->set_object(weak_from_this());
    }
}