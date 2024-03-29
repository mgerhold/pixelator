#pragma once

#ifndef ENABLE_PROFILING
#define ENABLE_PROFILING 0
#endif

#if ENABLE_PROFILING
#define SCOPED_TIMER_LINE2(line) \
    ScopedTimer _scoped_Timer##line { }
#define SCOPED_TIMER_LINE(line) SCOPED_TIMER_LINE2(line)
#define SCOPED_TIMER() SCOPED_TIMER_LINE(__LINE__)
#define SCOPED_TIMER_NAMED_LINE2(name, line) \
    ScopedTimer _scoped_Timer##line {        \
        name                                 \
    }
#define SCOPED_TIMER_NAMED_LINE(name, line) SCOPED_TIMER_NAMED_LINE2(name, line)
#define SCOPED_TIMER_NAMED(name) SCOPED_TIMER_NAMED_LINE(name, __LINE__)
#else
#define SCOPED_TIMER()
#define SCOPED_TIMER_NAMED(name)
#endif

#include <chrono>
#include <cstdint>
#include <source_location>
#include <string>
#include <unordered_map>

class ScopedTimer {
public:
    struct Measurement {
        std::uint64_t count;
        std::uint64_t depth;
        double minDuration;
        double maxDuration;
        double totalDuration;
    };

public:
    explicit
    ScopedTimer(char const* name = "", std::source_location sourceLocation = std::source_location::current()) noexcept;
    ~ScopedTimer();
    static void logResults() noexcept;

private:
    static std::string
    sourceLocationToString(std::string const& name, std::source_location const& sourceLocation) noexcept;

private:
    std::string mName;
    std::source_location mSourceLocation;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    static inline uint64_t sCurrentDepth{ 0ULL };
    static inline std::unordered_map<std::string, Measurement> sMeasurements{};
};
