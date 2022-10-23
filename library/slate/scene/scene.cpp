#include <iostream>

#include "scene.hpp"

namespace slate {
    void Scene::add(SlateObjectPtr object) {
        objects.insert(std::pair<std::string, SlateObjectPtr>(object->get_name(), object));
    }

    void Scene::add(std::shared_ptr<Light> light) {
        lights.push_back(light);
        add(std::dynamic_pointer_cast<SlateObject>(light));
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

    const std::vector<std::shared_ptr<Light>>& Scene::get_lights() const {
        return lights;
    }

    void Scene::update() {
        for (auto& o : objects) {
            o.second->update();
        }
    }
}