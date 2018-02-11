#include "rng.hpp"

namespace rn1
{
pl::RandomNumberGenerator<> &rng() noexcept
{
    thread_local pl::RandomNumberGenerator<> rng{ };
    return rng;
}
} // namespace rn1
