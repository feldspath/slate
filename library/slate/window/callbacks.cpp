#include "callbacks.hpp"

namespace slate {
    Callback& Callback::get() {
        static Callback singleton;
        return singleton;
    }
}