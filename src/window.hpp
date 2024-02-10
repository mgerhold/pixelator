#pragma once

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on
#include "opengl_version.hpp"
#include "window_size.hpp"
#include <string>

class Input;

class Window final {
public:
    Window(std::string const& title, WindowSize size, OpenGLVersion version, Input& input) noexcept;
    Window(Window const&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window const&) = delete;
    Window& operator=(Window&&) = delete;
    ~Window();

    [[nodiscard]] inline GLFWwindow* getGLFWWindowPointer() const noexcept {
        return mWindowPtr;
    }
    [[nodiscard]] WindowSize framebufferSize() const;

private:
    void initImGui() noexcept;
    void shutdownImGui() noexcept;
    static void handleOpenGLDebugOutput(
            GLenum source,
            GLenum type,
            unsigned int id,
            GLenum severity,
            GLsizei /* length */,
            char const* message,
            void const* /* userParam */
    );

private:
    GLFWwindow* mWindowPtr;
    WindowSize mFrameBufferSize;
    Input& mInput;
};
