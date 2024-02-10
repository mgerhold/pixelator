#include "random.hpp"

Random::Random() noexcept : mRandomEngine{ mRandomDevice() } { }
