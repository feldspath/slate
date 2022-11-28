#pragma once

#include <vector>
#include <memory>

#include <slate/event/subject.hpp>

namespace slate {
    class Callback {
    public:
        Subject window_resize;
        Subject mouse_move;
        Subject chronos;

        static Callback& get();

    private:
        Callback() {}
    };
}