#include <memory>
#include <iostream>

#include "scene.hpp"

namespace slate {
    void Scene::add_instance(std::string name, MeshPtr instance, glm::mat4 transform) {
        instances.insert(std::pair<std::string, InstancePtr>(name, std::make_shared<Instance>(instance, transform)));
    }

    InstancePtr Scene::get_instance_by_name(std::string name) {
        auto instance = instances.find(name);
        if (instance == instances.end()) {
            std::cerr << "Error::Scene: instance " << name << " not found in scene\n";
        }

        return instance->second;
    }

    const std::map<std::string, InstancePtr>& Scene::get_instances_map() {
        return instances;
    }



}