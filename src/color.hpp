#pragma once

#include "include_glm.hpp"

struct Color : public glm::vec4 {
    using glm::vec4::vec4;

    static constexpr Color white() {
        return Color{ 1.0f, 1.0f, 1.0f, 1.0f };
    }
};
