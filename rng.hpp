#ifndef INCG_RN1_RNG_HPP
#define INCG_RN1_RNG_HPP
#include <pl/random_number_generator.hpp>

namespace rn1
{
pl::RandomNumberGenerator<> &rng() noexcept;
} // namespace rn1
#endif // INCG_RN1_RNG_HPP
