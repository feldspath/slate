#pragma once

#include <memory>

#include "event.hpp"

namespace slate {
    class Observer : public std::enable_shared_from_this<Observer> {
    public:
        virtual void on_notify(Event event) = 0;
    };
}