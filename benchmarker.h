#ifndef BENCHMARKING_BENCHMARKER_H
#define BENCHMARKING_BENCHMARKER_H

#include <chrono>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_CLEAR_LINE    "\33[2K"

#define RED(x)     ANSI_COLOR_RED x ANSI_COLOR_RESET
#define GREEN(x)   ANSI_COLOR_GREEN x ANSI_COLOR_RESET
#define YELLOW(x)  ANSI_COLOR_YELLOW x ANSI_COLOR_RESET
#define BLUE(x)    ANSI_COLOR_BLUE x ANSI_COLOR_RESET
#define MAGENTA(x) ANSI_COLOR_MAGENTA x ANSI_COLOR_RESET
#define CYAN(x)    ANSI_COLOR_CYAN x ANSI_COLOR_RESET

namespace _benchmark {
    struct Stat {
        std::string profileName;
        uint32_t    iterations;
        double      mean;
        double      min;
        double      max;
    };

    class Session {
    public:
        explicit Session(std::string sessionName): mSession(std::move(sessionName)) {}

        ~Session() {
            printf("|----------------------------------|------------------------------------------------------------------|------------|--------------|--------------|--------------|\n");
            printf(
                "| "
                MAGENTA("Session") "                          | "
                MAGENTA("Scope") "                                                            | "
                MAGENTA("Iterations") " | "
                "         " MAGENTA("Avg") " | "
                "         " MAGENTA("Min") " | "
                "         " MAGENTA("Max") " |\n"
            );
            printf("|----------------------------------|------------------------------------------------------------------|------------|--------------|--------------|--------------|\n");

            for(const auto &stat: stats) {
                printf(
                    "| " CYAN("%-32.32s") " | " YELLOW("%-64.64s") " | %-10u | " BLUE("%10.2f") "ms | " GREEN("%10.2f") "ms | " RED("%10.2f") "ms |\n",
                    mSession.c_str(),
                    stat.profileName.c_str(),
                    stat.iterations,
                    stat.mean,
                    stat.min,
                    stat.max
                );
            }

            printf("|----------------------------------|------------------------------------------------------------------|------------|--------------|--------------|--------------|\n\n");
        }

        void log(Stat &&stat) {
            stats.emplace_back(stat);
        }

    private:
        std::string mSession;
        std::vector<Stat> stats;
    };
}

#define BENCHMARK_SESSION(sessionName, scope) do { \
    _benchmark::Session session(sessionName);      \
    scope                                          \
} while(0)

#define PROFILE_ME(ITERATIONS, func)                                                         \
do {                                                                                         \
    double elapsedTime[ITERATIONS];                                                          \
    for(volatile int i = 0; i < ITERATIONS; i=i+1) {                                         \
        printf(ANSI_CLEAR_LINE RED("[%s]") " Iteration : %u/%u\r", #func, i+1, ITERATIONS);  \
        fflush(stdout);                                                                      \
        auto start = std::chrono::high_resolution_clock::now();                              \
        volatile auto val = func;                                                            \
        elapsedTime[i] = double(std::chrono::duration_cast<std::chrono::nanoseconds>(        \
            std::chrono::high_resolution_clock::now() - start                                \
        ).count()) / 1.e6;                                                                   \
    }                                                                                        \
    session.log(_benchmark::Stat{                                                            \
        .profileName = #func,                                                                \
        .iterations = ITERATIONS,                                                            \
        .mean = std::accumulate(elapsedTime, elapsedTime+ITERATIONS, 0.0)/double(ITERATIONS),\
        .min = *std::min_element(elapsedTime, elapsedTime+ITERATIONS),                       \
        .max = *std::max_element(elapsedTime, elapsedTime+ITERATIONS)                        \
    });                                                                                      \
} while(0)

#endif //BENCHMARKING_BENCHMARKER_H
