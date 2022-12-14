#include <memory>
#include <slate/display/window/window.hpp>

namespace slate {

    class Input {
    private:
        std::weak_ptr<Window> window;
        Input() {}

    public:
        static Input& get();
        bool key_pressed(unsigned int key) const;
        void set_window(std::weak_ptr<Window> window);
    };
}