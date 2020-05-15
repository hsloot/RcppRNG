#ifndef RCPPRNG_RNG_RCPPRNG_EXP_HPP
#define RCPPRNG_RNG_RCPPRNG_EXP_HPP

#include <Rcpp.h>
#include <RcppRNG/RNG/RcppRNG/RNG.hpp>
#include <RcppRNG/generator/exp.hpp>

namespace RcppRNG {

template<>
inline double ExpGenerator<RcppRNG>::operator()() const {
  return R::exp_rand() * scale_;
}

}

#endif // RCPPRNG_RNG_RCPPRNG_EXP_HPP
