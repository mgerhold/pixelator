#include "application.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include <variant>

namespace {
    // anonymous namespace for functions and data that deal with measuring the frame time
    using namespace std::chrono_literals;

    constexpr auto frameTimeOutputInterval = 500ms;

    struct FrameTimeMeasurements {
        std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTimeOutput{};
        std::chrono::time_point<std::chrono::high_resolution_clock> lastTime{};
        unsigned long numFramesDuringOutputInterval{};
    };

    [[nodiscard]] FrameTimeMeasurements setupTimeMeasurements() noexcept {
        return FrameTimeMeasurements{ .lastFrameTimeOutput{ std::chrono::high_resolution_clock::now()
                                                            - frameTimeOutputInterval },
                                      .lastTime{ std::chrono::high_resolution_clock::now() },
                                      .numFramesDuringOutputInterval{ 0UL } };
    }

    void makeTimeMeasurementsStep(FrameTimeMeasurements& measurements, Time& time) noexcept {
        ++measurements.numFramesDuringOutputInterval;
        auto const currentTime = std::chrono::high_resolution_clock::now();
        time.delta = std::chrono::duration<double>(currentTime - measurements.lastTime).count();
        time.elapsed += time.delta;
        measurements.lastTime = currentTime;
        if (currentTime >= measurements.lastFrameTimeOutput + frameTimeOutputInterval) {
            time.meanFramesPerSecond =
                    static_cast<double>(measurements.numFramesDuringOutputInterval)
                    / std::chrono::duration<double, std::ratio<1, 1>>(currentTime - measurements.lastFrameTimeOutput)
                              .count();
            measurements.lastFrameTimeOutput = currentTime;
            measurements.numFramesDuringOutputInterval = 0UL;
        }
    }

} // namespace

Application::Application(std::string const& title, WindowSize size, OpenGLVersion version) noexcept
    : mWindow{ title, size, version, mInput },
      mRenderer{ mWindow },
      mAppContext{ mTime, mInput, *this } { }

Application::~Application() noexcept {
    ScopedTimer::logResults();
}

void Application::run() noexcept {
#ifdef DEBUG_BUILD
    spdlog::info("This is the debug build");
#else
    spdlog::info("This is the release build");
#endif
    setup();
    auto timeMeasurements = setupTimeMeasurements();
    while (!glfwWindowShouldClose(mWindow.getGLFWWindowPointer())) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        renderImGui();
        ImGui::Render();

        update();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(mWindow.getGLFWWindowPointer());
        mInput.nextFrame();
        glfwPollEvents();
        makeTimeMeasurementsStep(timeMeasurements, mTime);
        refreshWindowTitle();
    }
}

void Application::quit() noexcept {
    glfwSetWindowShouldClose(mWindow.getGLFWWindowPointer(), true);
}


void Application::refreshWindowTitle() noexcept {
    static std::string titleText = "";
    auto const targetTitleText =
            fmt::format("{:.2f} ms ({:.2f} fps)", mTime.meanFrameTime() * 1000.0, mTime.meanFramesPerSecond);
    if (titleText != targetTitleText) {
        glfwSetWindowTitle(mWindow.getGLFWWindowPointer(), targetTitleText.c_str());
        titleText = targetTitleText;
    }
}
