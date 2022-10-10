#include <iostream>

#include "scene.hpp"

namespace slate {
    void Scene::add_slate_object(SlateObjectPtr object) {
        objects.insert(std::pair<std::string, SlateObjectPtr>(object->get_name(), object));
    }

    SlateObjectPtr Scene::slate_object_by_name(std::string name) {
        auto obj = objects.find(name);
        if (obj == objects.end()) {
            std::cerr << "Error::Scene: instance " << name << " not found in scene\n";
        }

        return obj->second;
    }

    const std::map<std::string, SlateObjectPtr>& Scene::get_map() {
        return objects;
    }
}