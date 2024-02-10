#include "hash.hpp"
#include <utility>

namespace hash {
    std::unordered_map<std::size_t, std::string> cachedHashNames;

    std::string_view getStringFromHash(std::size_t hash) noexcept {
        auto const it = cachedHashNames.find(hash);
        if (it == cachedHashNames.cend()) {
            return "<name to hash not found>";
        } else {
            return it->second;
        }
    }
} // namespace hash
