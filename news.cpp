#include "news.hpp"
#include <cstddef> // std::size_t
#include <ostream> // std::ostream
#include <array> // std::array
#include <type_traits> // std::underlying_type_t

namespace rn1
{
std::ostream &operator<<(std::ostream &os, News news)
{
    constexpr std::size_t amtNews{ 3U };
    constexpr std::size_t strByteLen{ 5U };
    constexpr std::array<const char[strByteLen], amtNews> array{ {
        "INFO", "WARN", "CORR"
    } };

    return os << array[static_cast<std::underlying_type_t<News>>(news)];
}
} // namespace rn1
