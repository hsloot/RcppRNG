#ifndef RCPPRNG_HPP
#define RCPPRNG_HPP

#include <RcppRNG/RNG/DQRNG/DQRNG.hpp>
#include <RcppRNG/RNG/DQRNG/unit_exponential_distribution.hpp>
#include <RcppRNG/RNG/RNG.hpp>
#include <RcppRNG/RNG/RNGScope/unit_exponential_distribution.hpp>
#include <RcppRNG/RNG/RcppRNG/RcppRNG.hpp>
#include <RcppRNG/RNG/RcppRNG/unit_exponential_distribution.hpp>
#include <RcppRNG/distribution/exponential_distribution.hpp>
#include <RcppRNG/distribution/unit_exponential_distribution.hpp>

namespace RcppRNG {

template <typename _RealType>
using unit_exponential_distribution =
    distribution::unit_exponential_distribution<_RealType>;

template <typename _RealType, typename _UnitExponentialDistributionType>
using exponential_distribution =
    distribution::exponential_distribution<_RealType,
                                           _UnitExponentialDistributionType>;

}  // namespace RcppRNG

#endif  // RCPPRNG_HPP
