#ifndef RCPPRNG_DISTRIBUTION_EXP_HPP
#define RCPPRNG_DISTRIBUTION_EXP_HPP

#include <RcppRNG/distribution/distribution.hpp>

namespace RcppRNG {

template<typename RealType = double>
class ExpDistribution : public UnivariateDistribution {
public:
  ExpDistribution();
  ExpDistribution(RealType rate);

  virtual ~ExpDistribution() {}

  RealType rate() const;
  RealType scale() const;
private:
  inline void check_params();
  RealType scale_;
}; // ExpDistribution

template<typename RealType>
inline void ExpDistribution<RealType>::check_params() {
  if (scale_ < 0.)
    throw std::range_error("rate has to be non-negative");
}

template<typename RealType>
ExpDistribution<RealType>::ExpDistribution() :
    scale_(1.) {
  check_params();
}

template<typename RealType>
ExpDistribution<RealType>::ExpDistribution(RealType rate) :
    scale_(1. / rate) {
  check_params();
}

template<typename RealType>
RealType ExpDistribution<RealType>::rate() const {
  return 1. / scale_;
}

template<typename RealType>
RealType ExpDistribution<RealType>::scale() const {
  return scale_;
}

} // RcppRNG

#endif // RCPPRNG_DISTRIBUTION_EXP_HPP
