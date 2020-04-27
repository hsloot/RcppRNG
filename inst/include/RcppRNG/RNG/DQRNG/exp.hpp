#ifndef RCPPRNG_RNG_DQRNG_EXP_HPP
#define RCPPRNG_RNG_DQRNG_EXP_HPP

#include <Rcpp.h>
#include <RcppRNG/RNG/DQRNG/RNG.hpp>
#include <RcppRNG/generator/exp.hpp>
#include <dqrng_distribution.h>

namespace RcppRNG {

template<>
class ExpGenerator<DQRNG> : public Generator<DQRNG, double> {
public:
  ExpGenerator();
  ExpGenerator(double rate = 1.);

  inline double operator()() const;
private:
  dqrng::exponential_distribution exp;
  const DQRNG rng;
}; // ExpGenerator<T, double>

ExpGenerator<DQRNG>::ExpGenerator() :
  ExpGenerator(1.) {}

ExpGenerator<DQRNG>::ExpGenerator(double rate) {
  using parm_t = decltype(exp)::param_type;
  exp.param(parm_t(rate));
}

inline double ExpGenerator<DQRNG>::operator()() const {
  return exp(*rng.shared_rng);
}

} // RcppRNG

#endif // RCPPRNG_RNG_DQRNG_EXP_HPP
