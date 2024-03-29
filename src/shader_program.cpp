#include "shader_program.hpp"
#include "hash/hash.hpp"
#include <cassert>
#include <fstream>
#include <spdlog/spdlog.h>

GLuint ShaderProgram::sCurrentlyBoundName{ 0U };

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept {
    using std::swap;
    swap(mName, other.mName);
    swap(mUniformLocations, other.mUniformLocations);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
    using std::swap;
    swap(mName, other.mName);
    swap(mUniformLocations, other.mUniformLocations);
    return *this;
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(mName);
}

bool ShaderProgram::compile(std::string const& vertexShaderSource, std::string const& fragmentShaderSource) noexcept {
    if (hasBeenCompiled()) {
        glDeleteProgram(mName);
        mName = 0U;
    }
    GLuint vertexShaderName = glCreateShader(GL_VERTEX_SHADER);
    GLchar const* vertexShaderSourcesArray[] = { vertexShaderSource.c_str() };
    glShaderSource(vertexShaderName, 1U, vertexShaderSourcesArray, nullptr);
    glCompileShader(vertexShaderName);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShaderName, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShaderName, sizeof(infoLog), nullptr, infoLog);
        spdlog::error("Failed to compile vertex shader: {}", infoLog);
        glDeleteShader(vertexShaderName);
        return false;
    }

    GLuint fragmentShaderName = glCreateShader(GL_FRAGMENT_SHADER);
    GLchar const* fragmentShaderSourcesArray[] = { fragmentShaderSource.c_str() };
    glShaderSource(fragmentShaderName, 1U, fragmentShaderSourcesArray, nullptr);
    glCompileShader(fragmentShaderName);
    glGetShaderiv(fragmentShaderName, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderName, sizeof(infoLog), nullptr, infoLog);
        spdlog::error("Failed to compile fragment shader: {}", infoLog);
        glDeleteShader(vertexShaderName);
        glDeleteShader(fragmentShaderName);
        return false;
    }

    this->mName = glCreateProgram();
    glAttachShader(this->mName, vertexShaderName);
    glAttachShader(this->mName, fragmentShaderName);
    glLinkProgram(this->mName);

    glGetProgramiv(this->mName, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->mName, sizeof(infoLog), nullptr, infoLog);
        spdlog::error("Failed to link shader program: {}", infoLog);
        glDeleteShader(vertexShaderName);
        glDeleteShader(fragmentShaderName);
        glDeleteProgram(this->mName);
        return false;
    }

    glDeleteShader(vertexShaderName);
    glDeleteShader(fragmentShaderName);
    cacheUniformLocations();
    spdlog::info("Successfully linked shader program.");
    return true;
}

void ShaderProgram::bind(GLuint shaderName) noexcept {
    if (sCurrentlyBoundName != shaderName) {
        glUseProgram(shaderName);
        sCurrentlyBoundName = shaderName;
    }
}

void ShaderProgram::bind() const noexcept {
    bind(mName);
}

void ShaderProgram::unbind() noexcept {
    bind(0U);
}

tl::expected<ShaderProgram, std::string> ShaderProgram::generateFromFiles(
        std::filesystem::path const& vertexShaderPath,
        std::filesystem::path const& fragmentShaderPath
) {
    using namespace std::literals::string_literals;
    if (!exists(vertexShaderPath)) {
        return tl::unexpected{
            fmt::format("Vertex shader source file not found (filename was {}).", vertexShaderPath.string())
        };
    }
    if (!exists(fragmentShaderPath)) {
        return tl::unexpected{
            fmt::format("Fragment shader source file not found (filename was {}).", fragmentShaderPath.string())
        };
    }
    auto const readStringFromFile = [](std::filesystem::path const& path) {
        std::ifstream vertexShaderFileStream{ path };
        return std::string{ std::istreambuf_iterator<char>{ vertexShaderFileStream },
                            std::istreambuf_iterator<char>{} };
    };
    std::string vertexShaderSource = readStringFromFile(vertexShaderPath);
    std::string fragmentShaderSource = readStringFromFile(fragmentShaderPath);

    ShaderProgram shaderProgram;
    if (!shaderProgram.compile(vertexShaderSource, fragmentShaderSource)) {
        return tl::unexpected{ "Failed to create shader program."s };
    }
    return shaderProgram;
}

void ShaderProgram::setUniform(std::size_t uniformNameHash, glm::mat4 const& matrix) const noexcept {
    bind();
    auto const it = mUniformLocations.find(uniformNameHash);

    if (it == mUniformLocations.cend()) {
#ifdef DEBUG_BUILD
        spdlog::error(
                "Could not set uniform \"{}\" since it could not be found.",
                hash::getStringFromHash(uniformNameHash)
        );
#else
        spdlog::error("Could not set uniform because the hash value {} could not be found.", uniformNameHash);
#endif
        return;
    }
    glUniformMatrix4fv(it->second, 1, false, glm::value_ptr(matrix));
}

void ShaderProgram::cacheUniformLocations() noexcept {
    GLint uniform_count = 0;
    glGetProgramiv(this->mName, GL_ACTIVE_UNIFORMS, &uniform_count);

    if (uniform_count != 0) {
        GLint max_name_len = 0;
        GLsizei length = 0;
        GLsizei count = 0;
        GLenum type = GL_NONE;
        glGetProgramiv(this->mName, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

        auto uniform_name = std::make_unique<char[]>(max_name_len);

        for (GLint i = 0; i < uniform_count; ++i) {
            glGetActiveUniform(this->mName, i, max_name_len, &length, &count, &type, uniform_name.get());

            GLint location = glGetUniformLocation(this->mName, uniform_name.get());

            std::size_t const hash =
                    hash::hashString(std::string_view{ uniform_name.get(), static_cast<std::size_t>(length) });
            mUniformLocations[hash] = location;
#ifdef DEBUG_BUILD
            spdlog::info("uniform location for \"{}\" (hash: {:X}): {}", uniform_name.get(), hash, location);
#endif
        }
    }
}

ShaderProgram ShaderProgram::defaultProgram() noexcept {
    std::string const vertexShader = R"(#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in uint aTexIndex;

out vec4 fragmentColor;
out vec3 fragmentPosition;
out vec2 texCoords;
flat out uint texIndex;

uniform mat4 projectionMatrix;

void main() {
   vec4 position = projectionMatrix * vec4(aPos.xyz, 1.0);
   fragmentPosition = position.xyz;
   fragmentColor = aColor;
   texCoords = aTexCoords;
   texIndex = aTexIndex;
   gl_Position = position;
})";
    std::string const fragmentShader = R"(#version 450 core

in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec2 texCoords;
flat in uint texIndex;

out vec4 FragColor;

layout (binding = 0) uniform sampler2D uTextures[32];

void main() {
    vec4 color = texture(uTextures[texIndex], texCoords) * fragmentColor;
    if (color.a == 0.0) {
        discard;
    }
    FragColor = color;
})";
    ShaderProgram result;
    [[maybe_unused]] bool const success = result.compile(vertexShader, fragmentShader);
    assert(success);
    return result;
}
