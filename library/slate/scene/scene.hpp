#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "object/slate_object.hpp"

namespace slate {
    class Scene {
    private:
        std::map<std::string, SlateObjectPtr> objects;

    public:
        void add_slate_object(SlateObjectPtr object);
        SlateObjectPtr slate_object_by_name(std::string name);
        const std::map<std::string, SlateObjectPtr>& get_map();
        void update();

        template <typename T>
        std::vector<std::shared_ptr<T>> components_by_type() const;
    };

    template <typename T>
    std::vector<std::shared_ptr<T>> Scene::components_by_type() const {
        std::vector<std::shared_ptr<T>> components;
        for (auto& obj : objects) {
            auto comp = obj.second->get_component<T>();
            if (comp) components.push_back(comp);
        }
        return components;
    }
}