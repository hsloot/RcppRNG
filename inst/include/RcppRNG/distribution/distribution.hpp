#ifndef RCPPRNG_DISTRIBUTION_DISTRIBUTION_HPP
#define RCPPRNG_DISTRIBUTION_DISTRIBUTION_HPP

namespace RcppRNG {

class Distribution {
 public:
  virtual ~Distribution() {}
};  // Distribution

class UnivariateDistribution : public Distribution {
};  // UnivariateDistribution

}  // namespace RcppRNG

#endif  // RCPPRNG_DISTRIBUTION_DISTRIBUTION_HPP
