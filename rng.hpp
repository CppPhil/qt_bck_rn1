#ifndef INCG_RN1_RNG_HPP
#define INCG_RN1_RNG_HPP
#include <pl/random_number_generator.hpp> // pl::RandomNumberGenerator

namespace rn1
{
/*!
 * \brief Returns a reference to the thread_local pl::RandomNumberGenerator
 * \return A reference to the thread_local pl::RandomNumberGenerator
**/
pl::RandomNumberGenerator<> &rng() noexcept;
} // namespace rn1
#endif // INCG_RN1_RNG_HPP
