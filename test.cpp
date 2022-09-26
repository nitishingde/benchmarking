#include "benchmarker.h"

uint64_t fib(uint64_t no) {
    if(no == 0) return 0;
    if(no == 1) return 1;

    return fib(no-1) + fib(no-2);
}

int main([[maybe_unused]]int argc, [[maybe_unused]]char *argv[]) {
    BENCHMARK_SESSION("Fibonacci", {
        PROFILE_ME(10, fib(38));
        PROFILE_ME(10, fib(42));
    });

    return 0;
}
