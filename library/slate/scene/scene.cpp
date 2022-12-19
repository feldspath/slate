#include <iostream>

#include "scene.hpp"

namespace slate {
    void Scene::add(SlateObjectPtr object) {
        auto collision = objects.find(object->get_name());
        if (collision != objects.end()) {
            std::cerr << "Error::Scene::add: cannot add object, the name " << object->get_name() << " is already taken\n";
            return;
        }
        objects.insert(std::pair<std::string, SlateObjectPtr>(object->get_name(), object));
    }

    void Scene::add(std::shared_ptr<Light> light) {
        lights.push_back(light);
        add(std::dynamic_pointer_cast<SlateObject>(light));
    }

    void Scene::add(CameraPtr camera) {
        this->camera = camera;
        add(std::dynamic_pointer_cast<SlateObject>(camera));
    }

    const CameraPtr Scene::get_camera() const {
        if (!camera) {
            std::cerr << "Error::Scene::get_camera: the scene camera is not set\n";
            return nullptr;
        }
        return camera;
    }

    void Scene::rename_object(const std::string& old_name, const std::string& new_name) {
        auto obj = slate_object_by_name(old_name);
        if (!obj) return;

        auto collision = objects.find(new_name);
        if (collision != objects.end()) {
            std::cerr << "Error::Scene::rename_object: cannot rename object, the name " << new_name << " is already taken\n";
            return;
        }

        auto node = objects.extract(old_name);
        node.key() = new_name;
        objects.insert(std::move(node));
    }

    void Scene::rename_object(SlateObjectPtr object, const std::string& new_name) {
        std::string name = object->get_name();
        auto obj = slate_object_by_name(name);
        if (!obj) return;
        rename_object(name, new_name);
    }

    SlateObjectPtr Scene::slate_object_by_name(std::string name) {
        auto obj = objects.find(name);
        if (obj == objects.end()) {
            std::cerr << "Error::Scene: instance " << name << " not found in scene\n";
            return nullptr;
        }

        return obj->second;
    }

    const std::map<std::string, SlateObjectPtr>& Scene::get_map() {
        return objects;
    }

    const std::vector<std::shared_ptr<Light>>& Scene::get_lights() const {
        return lights;
    }

    void Scene::update(const float dt) {
        for (auto& o : objects) {
            o.second->update(shared_from_this(), dt);
        }
    }
}