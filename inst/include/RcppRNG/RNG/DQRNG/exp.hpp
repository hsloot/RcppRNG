#ifndef RCPPRNG_RNG_DQRNG_EXP_HPP
#define RCPPRNG_RNG_DQRNG_EXP_HPP

#include <Rcpp.h>
#include <dqrng_distribution.h>

#include <RcppRNG/RNG/DQRNG/RNG.hpp>
#include <RcppRNG/distribution/exp.hpp>
#include <RcppRNG/generator/exp.hpp>

namespace RcppRNG {

template <>
class Generator<DQRNG, double, ExpDistribution<double>> {
 public:
  Generator();
  Generator(const ExpDistribution<double> &param);

  virtual ~Generator() {}

  inline double operator()() const;

 private:
  ExpDistribution<double> param_;
  dqrng::exponential_distribution exp_;
  const DQRNG rng_;
};  // Generator<DQRNG, double, ExpDistribution>

ExpGenerator<DQRNG>::Generator() {
  using parm_t = decltype(exp_)::param_type;
  exp_.param(parm_t(param_.rate()));
}

ExpGenerator<DQRNG>::Generator(const ExpDistribution<double> &param)
    : param_(param) {
  using parm_t = decltype(exp_)::param_type;
  exp_.param(parm_t(param_.rate()));
}

inline double ExpGenerator<DQRNG>::operator()() const {
  return exp_(*rng_.shared_rng);
}

}  // namespace RcppRNG

#endif  // RCPPRNG_RNG_DQRNG_EXP_HPP
