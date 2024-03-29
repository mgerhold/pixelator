//
// Created by coder2k on 05.06.2021.
//

#pragma once

#include "color.hpp"
#include "guid.hpp"
#include "image.hpp"
#include <glad/gl.h>
#include <tl/expected.hpp>


class Texture final {
public:
    enum class Filtering {
        Linear,
        Nearest,
    };

    Texture() = default;

    Texture(Texture const&) = delete;
    Texture(Texture&& other) noexcept;
    ~Texture();

    Texture& operator=(Texture const&) = delete;
    Texture& operator=(Texture&& other) noexcept;


    void bind(GLint textureUnit = 0U) const noexcept;
    static void unbind(GLint textureUnit) noexcept;
    void setFiltering(Filtering filtering) const noexcept;
    void setWrap(bool enabled) const noexcept;
    [[nodiscard]] int width() const noexcept {
        return mWidth;
    }
    [[nodiscard]] int height() const noexcept {
        return mHeight;
    }
    [[nodiscard]] float widthToHeightRatio() const noexcept {
        return static_cast<float>(mWidth) / static_cast<float>(mHeight);
    }
    [[nodiscard]] int numChannels() const noexcept {
        return mNumChannels;
    }

    [[nodiscard]] static tl::expected<Texture, std::string> create(Image const& image) noexcept;
    [[nodiscard]] static tl::expected<Texture, std::string>
    createFromMemory(int width, int height, int numChannels, unsigned char* data) noexcept;
    [[nodiscard]] static tl::expected<Texture, std::string>
    createFromFillColor(int width, int height, int numChannels, Color fillColor) noexcept;
    [[nodiscard]] static GLint getTextureUnitCount() noexcept;

public:
    GUID guid;

private:
    static void bind(GLuint textureName, GLint textureUnit) noexcept;

private:
    static inline GLint sTextureUnitCount{ 0U };
    int mWidth{ 0U };
    int mHeight{ 0U };
    int mNumChannels{ 0U };
    GLuint mName{ 0U };

    friend class Renderer;
};
