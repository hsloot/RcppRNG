#ifndef RCPPRNG_RNG_RCPSCOPE_EXP_HPP
#define RCPPRNG_RNG_RCPSCOPE_EXP_HPP

#include <Rcpp.h>
#include <RcppRNG/generator/exp.hpp>

namespace RcppRNG {

template<>
inline double ExpGenerator<Rcpp::RNGScope>::operator()() const {
  return R::exp_rand() * param_.scale();
}

} // RcppRNG

#endif // RCPPRNG_RNG_RCPPRNG_EXP_HPP
