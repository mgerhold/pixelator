#pragma once

#include "color.hpp"
#include <filesystem>
#include <tl/expected.hpp>

class Image final {
public:
    Image(Image const&) = delete;
    Image(Image&& other) noexcept;

    Image& operator=(Image const&) = delete;
    Image& operator=(Image&& other) noexcept;

    [[nodiscard]] static tl::expected<Image, std::string>
    loadFromFile(std::filesystem::path const& filename, int numChannels = 0) noexcept;
    [[nodiscard]] int getWidth() const noexcept;
    [[nodiscard]] int getHeight() const noexcept;
    [[nodiscard]] int getNumChannels() const noexcept;
    [[nodiscard]] unsigned char* getData() const noexcept;

private:
    struct Deleter {
        void operator()(unsigned char* data);
    };
    using Pointer = std::unique_ptr<unsigned char, Deleter>;

private:
    Image() = default;

private:
    int mWidth{ 0 };
    int mHeight{ 0 };
    int mNumChannels{ 0 };
    Pointer mData{ nullptr };
};
