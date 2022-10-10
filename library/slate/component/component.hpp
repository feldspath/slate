#pragma once

#include <memory>

namespace slate {
    // class ComponentInfo {
    // private:
    //     template <typename T>
    //     struct registered {
    //         static const unsigned int index;
    //     };
        
    //     template <typename T>
    //     static inline unsigned int get_index() {
    //         return registered<T>::index;
    //     }
    // };

    
    // template <typename T>
    // const unsigned int ComponentInfo::registered<T>::index = ComponentInfo::add_type(1);

    class SlateObject;

    class Component {
    protected:
        std::weak_ptr<SlateObject> target;

    public:
        virtual void update() = 0;
        void set_object(std::weak_ptr<SlateObject> target);
    };

    using ComponentPtr = std::shared_ptr<Component>;
}