#pragma once

#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "slate/mesh/mesh.hpp"
#include "slate/instance/instance.hpp"

namespace slate {
    class Scene {
    private:
        std::vector<MeshPtr> meshes;
        std::map<std::string, InstancePtr> instances;

    public:
        void add_instance(std::string name, MeshPtr instance, glm::mat4 transform = glm::mat4(1.0f));
        InstancePtr get_instance_by_name(std::string name);
        const std::map<std::string, InstancePtr>& get_instances_map();
    };
}