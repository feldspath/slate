#pragma once

#include <vector>
#include <memory>

#include "observer.hpp"
#include "event.hpp"

namespace slate {
    class Subject {
    private:
        std::vector<std::weak_ptr<Observer>> observers;

    public:
        void add_observer(std::weak_ptr<Observer> observer);
        void notify(Event event) const;
    };
}