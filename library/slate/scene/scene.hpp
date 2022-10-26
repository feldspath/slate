#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "object/slate_object.hpp"
#include "light/light_base.hpp"
#include "camera/camera_base.hpp"

namespace slate {
    class Scene {
    private:
        std::map<std::string, SlateObjectPtr> objects;
        std::vector<std::shared_ptr<Light>> lights;

        CameraPtr camera;

    public:
        void add(SlateObjectPtr object);
        void add(std::shared_ptr<Light> light);
        void add(CameraPtr camera);
        void rename_object(const std::string& old_name, const std::string& new_name);
        void rename_object(SlateObjectPtr object, const std::string& new_name);

        SlateObjectPtr slate_object_by_name(std::string name);

        const std::map<std::string, SlateObjectPtr>& get_map();
        const std::vector<std::shared_ptr<Light>>& get_lights() const;
        const CameraPtr get_camera() const;

        void update(const float dt);

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