#ifndef INCG_RN1_NEWS_HPP
#define INCG_RN1_NEWS_HPP
#include <cstdint> // std::uint16_t, UINT16_C
#include <iosfwd> // std::ostream

namespace rn1
{
enum class News : std::uint16_t
{
    info = UINT16_C(0),
    warn = UINT16_C(1),
    corr = UINT16_C(2)
};

std::ostream &operator<<(std::ostream &os, News news);
} // namespcace rn1
#endif // INCG_RN1_NEWS_HPP
