#pragma once

struct Time {
    double elapsed{ 1.0 / 60.0 };
    double delta{ 1.0 / 60.0 };
    double meanFramesPerSecond{ 1.0 / 60.0 };

    [[nodiscard]] double meanFrameTime() const noexcept {
        return 1.0 / meanFramesPerSecond;
    }
};