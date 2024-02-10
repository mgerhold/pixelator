#pragma once

struct Rect {
    float left;
    float bottom;
    float right;
    float top;

    [[nodiscard]] static Rect unit() noexcept {
        return Rect{ .left{ 0.0f }, .bottom{ 0.0f }, .right{ 1.0f }, .top{ 1.0f } };
    }
};
