#include <algorithm>
#include <iostream>

#include "subject.hpp"

namespace slate {
    void Subject::add_observer(std::weak_ptr<Observer> observer) {
        observers.push_back(observer);
    }
    
    void Subject::notify(Event event) const {
        for (auto& observer : observers) {
            auto observer_shared = observer.lock();
            if (!observer_shared) {
                std::cerr << "Warning::Subject: Expired weak pointer to observer of event " << static_cast<int>(event.type) << '\n';
                continue;
            }
            observer_shared->on_notify(event);
        }
    }    
}

