#pragma once

#include "include_glm.hpp"
#include <glad/gl.h>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <tl/expected.hpp>

class ShaderProgram final {
public:
    ShaderProgram() = default;
    ShaderProgram(ShaderProgram const&) = delete;
    ShaderProgram(ShaderProgram&& other) noexcept;

    ShaderProgram& operator=(ShaderProgram const&) = delete;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    ~ShaderProgram();

    [[nodiscard]] bool compile(std::string const& vertexShaderSource, std::string const& fragmentShaderSource) noexcept;
    static void bind(GLuint shaderName) noexcept;
    void bind() const noexcept;
    static void unbind() noexcept;
    [[nodiscard]] bool hasBeenCompiled() const noexcept {
        return mName != 0U;
    }
    static tl::expected<ShaderProgram, std::string>
    generateFromFiles(std::filesystem::path const& vertexShaderPath, std::filesystem::path const& fragmentShaderPath);
    static void setUniform(GLuint shaderName, std::size_t uniformNameHash, glm::mat4 const& matrix) noexcept;
    void setUniform(std::size_t uniformNameHash, glm::mat4 const& matrix) const noexcept;
    [[nodiscard]] static ShaderProgram defaultProgram() noexcept;


private:
    void cacheUniformLocations() noexcept;

private:
    static GLuint sCurrentlyBoundName;
    GLuint mName{ 0U };
    std::unordered_map<std::size_t, GLint> mUniformLocations;

    friend class Renderer;
};
