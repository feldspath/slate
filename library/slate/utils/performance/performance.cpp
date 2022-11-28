#include "performance.hpp"

#include <slate/event/callbacks/callbacks.hpp>


namespace slate {
    Benchmark::Benchmark(std::string name) : name(name) {
        start_time = std::chrono::high_resolution_clock::now();
    }

    Benchmark::~Benchmark() {
        std::chrono::duration<double, std::milli> elapsed_time = std::chrono::high_resolution_clock::now() - start_time;
        Event e;
        e.type = EventType::CHRONO_DONE;
        e.add_float_arg(name, elapsed_time.count());
        Callback::get().chronos.notify(e);
    }
}