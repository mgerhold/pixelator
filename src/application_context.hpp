#pragma once

#include <vector>

struct Time;
class Input;
class Application;

struct ApplicationContext {
    ApplicationContext(
            Time& time,
            Input& input,
            Application& application
    ) noexcept;

    Time& time;
    Input& input;
    Application& application;
};
