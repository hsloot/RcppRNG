#ifndef RCPPRNG_DISTRIBUTION_EXP_HPP
#define RCPPRNG_DISTRIBUTION_EXP_HPP

#include <RcppRNG/distribution/distribution.hpp>

namespace RcppRNG {

class ExpDistribution : public UnivariateDistribution {
public:
  ExpDistribution();
  ExpDistribution(double rate);

  virtual ~ExpDistribution() {}

  double rate() const;
  double scale() const;
private:
  const double scale_;
}; // ExpDistribution

ExpDistribution::ExpDistribution() :
    scale_(1.) {}

ExpDistribution::ExpDistribution(double rate) :
    scale_(1. / rate) {}

double ExpDistribution::rate() const {
  return 1. / scale_;
}

double ExpDistribution::scale() const {
  return scale_;
}

} // RcppRNG

#endif // RCPPRNG_DISTRIBUTION_EXP_HPP
