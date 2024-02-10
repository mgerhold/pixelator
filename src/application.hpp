//
// Created by coder2k on 15.05.2021.
//

#pragma once

#include "application_context.hpp"
#include "input.hpp"
#include "opengl_version.hpp"
#include "random.hpp"
#include "renderer.hpp"
#include "scoped_timer.hpp"
#include "time.hpp"
#include "window.hpp"
#include "window_size.hpp"

class Application {
public:
    explicit Application(
            std::string const& title = "c2k application",
            WindowSize size = WindowSize{ .width{ 800 }, .height{ 600 } },
            OpenGLVersion version = OpenGLVersion{ .major{ 4 }, .minor{ 5 } }
    ) noexcept;
    Application(Application const&) = delete;
    Application(Application&&) = delete;
    virtual ~Application() noexcept;
    Application& operator=(Application const&) = delete;
    Application& operator=(Application&&) = delete;
    void run() noexcept;
    void quit() noexcept;

private:
    virtual void setup() noexcept = 0;
    virtual void update() noexcept = 0;
    virtual void renderImGui() noexcept { }
    void refreshWindowTitle() noexcept;

protected:
    Input mInput;
    Window mWindow;
    Renderer mRenderer;
    Time mTime;
    Random mRandom;
    ApplicationContext mAppContext;
};
