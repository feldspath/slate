#pragma once

#include <vector>
#include <memory>

#include "../event/subject.hpp"

namespace slate {
    class Callback {
    private:
        static Callback singleton;

    public:
        Subject window_resize;

        static Callback& get();
    };
}