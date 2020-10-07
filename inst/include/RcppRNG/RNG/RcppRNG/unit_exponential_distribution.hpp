#ifndef RCPPRNG_RNG_RCPPRNG_EXP_HPP
#define RCPPRNG_RNG_RCPPRNG_EXP_HPP

#include <Rcpp.h>

#include <RcppRNG/RNG/RcppRNG/RcppRNG.hpp>
#include <RcppRNG/distribution/unit_exponential_distribution.hpp>

namespace RcppRNG {

namespace distribution {

template <>
template <>
double unit_exponential_distribution<double>::operator()(
    RcppRNG::rng::RcppRNG& eng) {
  return R::exp_rand();
}

}  // namespace distribution

}  // namespace RcppRNG

#endif  // RCPPRNG_RNG_RCPPRNG_EXP_HPP
