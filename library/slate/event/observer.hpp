#pragma once

#include "event.hpp"

namespace slate {
    class Observer {
    public:
        virtual void on_notify(Event event) = 0;
    };
}