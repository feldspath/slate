#pragma once

#include <map>
#include <string>
#include <vector>

#include "object/slate_object.hpp"

namespace slate {
    class Scene {
    private:
        std::map<std::string, SlateObjectPtr> objects;

    public:
        void add_slate_object(SlateObjectPtr object);
        SlateObjectPtr slate_object_by_name(std::string name);
        const std::map<std::string, SlateObjectPtr>& get_map();
    };
}