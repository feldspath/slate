#pragma once

#include <slate/component/component.hpp>
#include <slate/component/transform/transform.hpp>

#include <string>
#include <memory>
#include <vector>
#include <iostream>

namespace slate {
    class SlateObject : public std::enable_shared_from_this<SlateObject> {
    private:
        unsigned int id;
        std::string name;
        std::vector<ComponentPtr> components;

        static unsigned int object_count;

    public:

        Transform transform;

        SlateObject(std::string name_);
        SlateObject();
        const std::string& get_name();
        void add_component(ComponentPtr component);
        template<typename T>
        std::shared_ptr<T> get_component();

        void update();
    };

    using SlateObjectPtr = std::shared_ptr<SlateObject>;

    template<typename T>
    std::shared_ptr<T> SlateObject::get_component() {
        for (auto c : components) {
            auto t_comp = std::dynamic_pointer_cast<T>(c);
            if (t_comp != nullptr) {
                return t_comp;
            }
        }
        return nullptr;
    }
}
