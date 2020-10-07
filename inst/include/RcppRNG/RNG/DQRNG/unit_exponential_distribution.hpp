#ifndef RCPPRNG_RNG_DQRNG_EXP_HPP
#define RCPPRNG_RNG_DQRNG_EXP_HPP

#include <RcppRNG/RNG/DQRNG/DQRNG.hpp>
#include <RcppRNG/distribution/unit_exponential_distribution.hpp>

namespace RcppRNG {

namespace distribution {

template <>
template <>
double unit_exponential_distribution<double>::operator()(
    rng::DQRNG& eng) {
  return eng.ued(*eng.shared_rng);
}

}  // namespace distribution

}  // namespace RcppRNG

#endif  // RCPPRNG_RNG_DQRNG_EXP_HPP
