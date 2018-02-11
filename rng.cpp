#include "rng.hpp"

namespace rn1
{
pl::RandomNumberGenerator<> &rng() noexcept
{
    // using thread_local storage
    thread_local pl::RandomNumberGenerator<> rng{ };
    return rng;
}
} // namespace rn1
