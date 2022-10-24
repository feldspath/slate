#pragma once

#include <slate/event/observer.hpp>
#include <memory>

namespace slate {
    class SlateObject;

    class Component : public Observer {
    private:
        std::weak_ptr<SlateObject> target_ptr;

    public:
        virtual void update() = 0;
        void set_object(std::weak_ptr<SlateObject> get_target);
        std::shared_ptr<SlateObject> get_target();
        virtual void on_notify(Event) {}
    };

    using ComponentPtr = std::shared_ptr<Component>;
}