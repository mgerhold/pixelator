#pragma once

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <random>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>


struct GUID {
    [[nodiscard]] bool operator==(const GUID&) const = default;

    [[nodiscard]] std::uint64_t low() const noexcept {
        return mLow;
    }

    [[nodiscard]] std::uint64_t high() const noexcept {
        return mHigh;
    }

    [[nodiscard]] static GUID create() noexcept {
        GUID result;
        result.mHigh = mDistribution(mRandomGenerator);
        result.mLow = mDistribution(mRandomGenerator);
        return result;
    }

    [[nodiscard]] static GUID fromString(std::string string) noexcept {
        string.erase(std::remove(string.begin(), string.end(), '-'), string.end());
        std::stringstream highStream;
        highStream << std::hex << string.substr(0, 16);
        std::stringstream lowStream;
        lowStream << std::hex << string.substr(16);
        GUID result;
        highStream >> result.mHigh;
        lowStream >> result.mLow;
        return result;
    }

    [[nodiscard]] std::string string() const noexcept {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(16) << mHigh << std::setw(16) << mLow;
        auto result = ss.str();
        result.insert(8, 1, '-');
        result.insert(13, 1, '-');
        result.insert(18, 1, '-');
        result.insert(23, 1, '-');
        return result;
    }

    [[nodiscard]] static GUID invalid() noexcept {
        static auto const result{ create() };
        return result;
    }

private:
    static inline std::random_device mRandomDevice{};
    static inline std::mt19937_64 mRandomGenerator{ mRandomDevice() };
    static inline std::uniform_int_distribution<std::uint64_t> mDistribution{};

    std::uint64_t mHigh{ 0 }, mLow{ 0 };
};

template<>
struct fmt::formatter<GUID> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        auto it = ctx.begin();
        return it;
    }

    template<typename FormatContext>
    auto format(const GUID& guid, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(), "{}", guid.string());
    }
};

namespace std {

    template<>
    struct hash<GUID> {
        size_t operator()(const GUID& guid) const noexcept {
            return guid.low() ^ guid.high();
        }
    };

} // namespace std
