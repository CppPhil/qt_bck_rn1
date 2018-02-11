#ifndef INCG_RN1_NEWS_HPP
#define INCG_RN1_NEWS_HPP
#include <cstdint> // std::uint16_t, UINT16_C
#include <iosfwd> // std::ostream

namespace rn1
{
/*!
 * \brief Scoped enum for the News-types.
 * \note using uint16_t as the underlying type as uint8_t is typically an
 *       alias for unsigned char for which random numbers cannot be
 *       generated ...
**/
enum class News : std::uint16_t
{
    /* UINT16_C creates a literal of types std::uint16_t */
    info = UINT16_C(0), /*!< The info News-type */
    warn = UINT16_C(1), /*!< The warning News-type */
    corr = UINT16_C(2) /*!< The correction News-type */
};

/*!
 * \brief Prints a News enumerator to an ostream.
 * \param os The ostream to print to.
 * \param news The News enumerator to print.
 * \return A reference to 'os'.
 * \note Called by boost::lexical_cast<std::string>
**/
std::ostream &operator<<(std::ostream &os, News news);
} // namespcace rn1
#endif // INCG_RN1_NEWS_HPP
