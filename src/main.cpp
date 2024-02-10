#include "application.hpp"
#include "include_glm.hpp"
#include "input.hpp"
#include "window.hpp"
#include <array>
#include <glm/ext/vector_common.hpp>

class TestApplication final : public Application {
private:
    glm::ivec2 m_resolution;
    std::vector<unsigned char> m_buffer;
    ShaderProgram m_shader_program{ ShaderProgram::defaultProgram() };
    Texture m_texture;

public:
    explicit TestApplication(glm::ivec2 const resolution)
        : m_resolution{ resolution },
          m_buffer(resolution.x * resolution.y * 4, 0) { }

private:
    void setup() noexcept override { }

    void update() noexcept override {
        auto const center = glm::vec2{ m_resolution.x / 2, m_resolution.y / 2 };
        auto const radius_offset_factor = static_cast<float>(std::sin(mTime.elapsed * 0.8)) * 0.3f + 1.0f;
        auto const radius = static_cast<float>(std::min(m_resolution.x, m_resolution.y) * 0.2) * radius_offset_factor;
        auto const movement_speed = 6.0f;
        auto const point = glm::vec2{
            center.x + radius * std::cos(mTime.elapsed * movement_speed),
            center.y + radius * std::sin(mTime.elapsed * movement_speed),
        };
        set_pixel(glm::ivec2{ static_cast<int>(point.x), static_cast<int>(point.y) }, Color::white());

        auto new_buffer = std::vector<unsigned char>(m_buffer.size(), 0);
        for (auto y = 1; y < m_resolution.y - 1; ++y) {
            for (auto x = 1; x < m_resolution.x - 1; ++x) {
                auto const position = glm::ivec2{ x, y };
                auto color_sum = glm::vec4{ 0.0f };
                for (auto v = y - 1; v <= y + 1; ++v) {
                    for (auto u = x - 1; u <= x + 1; ++u) {
                        color_sum += get_pixel(glm::ivec2{ u, v });
                    }
                }
                auto const average_color = color_sum / 9.0f;
                auto const frame_delta = static_cast<float>(mTime.delta);
                auto const rate = 0.2f;
                auto const smoothing = 1.0f - rate;
                auto const lerped = mix(get_pixel(position), average_color, 1.0f - std::pow(smoothing, frame_delta));

                auto const pixel_num = x + y * m_resolution.x;
                new_buffer.at(4 * pixel_num + 0) = static_cast<unsigned char>(lerped.r * 255.0f);
                new_buffer.at(4 * pixel_num + 1) = static_cast<unsigned char>(lerped.g * 255.0f);
                new_buffer.at(4 * pixel_num + 2) = static_cast<unsigned char>(lerped.b * 255.0f);
                new_buffer.at(4 * pixel_num + 3) = static_cast<unsigned char>(lerped.a * 255.0f);
            }
        }
        m_buffer = std::move(new_buffer);

        m_texture = Texture::createFromMemory(m_resolution.x, m_resolution.y, 4, m_buffer.data()).value();
        m_texture.setFiltering(Texture::Filtering::Nearest);
        mRenderer.beginFrame(glm::mat4{ 1.0 });
        mRenderer.setClearColor(Color{ 0.0f, 0.0f, 0.0f, 1.0f });
        mRenderer.clear(true, true);
        mRenderer.drawQuad(glm::vec3{ 0.0f }, 0.0f, glm::vec2{ 1.0f }, m_shader_program, m_texture);
        mRenderer.endFrame();
    }

    void set_pixel(glm::ivec2 const position, Color const color) {
        auto const pixel_num = position.x + position.y * m_resolution.x;
        m_buffer.at(4 * pixel_num + 0) = static_cast<unsigned char>(color.r * 255.0f);
        m_buffer.at(4 * pixel_num + 1) = static_cast<unsigned char>(color.g * 255.0f);
        m_buffer.at(4 * pixel_num + 2) = static_cast<unsigned char>(color.b * 255.0f);
        m_buffer.at(4 * pixel_num + 3) = static_cast<unsigned char>(color.a * 255.0f);
    }

    Color get_pixel(glm::ivec2 const position) const {
        auto const pixel_num = position.x + position.y * m_resolution.x;
        return Color{
            static_cast<float>(m_buffer.at(4 * pixel_num + 0)) / 255.0f,
            static_cast<float>(m_buffer.at(4 * pixel_num + 1)) / 255.0f,
            static_cast<float>(m_buffer.at(4 * pixel_num + 2)) / 255.0f,
            static_cast<float>(m_buffer.at(4 * pixel_num + 3)) / 255.0f,
        };
    }
};

int main() {
    auto application = TestApplication{
        //glm::ivec2{ 400, 300 }
        glm::ivec2{ 100, 75 }
    };
    application.run();
}
