#pragma once

#include <string>
#include <chrono>

namespace slate {
    class Benchmark {
    private:
        std::string name;
        std::chrono::high_resolution_clock::time_point start_time;

    public:
        Benchmark(std::string name);
        ~Benchmark();
    };
}