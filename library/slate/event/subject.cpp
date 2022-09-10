#include <algorithm>

#include "subject.hpp"

namespace slate {
    void Subject::add_observer(std::shared_ptr<Observer> observer) {
        observers.push_back(observer);
    }

    void Subject::remove_observer(std::shared_ptr<Observer> observer) {
        observers.erase(std::find(observers.begin(), observers.end(), observer));
    }
    
    void Subject::notify(Event event) const {
        for (auto& observer : observers) {
            observer->on_notify(event);
        }
    }
    
}

