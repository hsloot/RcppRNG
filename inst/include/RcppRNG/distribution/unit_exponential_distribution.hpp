#ifndef RCPPRNG_DISTRIBUTION_UNIT_EXPONENTIAL_HPP
#define RCPPRNG_DISTRIBUTION_UNIT_EXPONENTIAL_HPP

namespace RcppRNG {

namespace distribution {

template <typename _RealType = double>
class unit_exponential_distribution {
 public:
  using output_type = _RealType;

  // default ctor's and copy assignment operator are sufficient.

  template <typename _EngineType>
  _RealType operator()(_EngineType& eng);
};

}  // namespace distribution

}  // namespace RcppRNG

#endif  // RCPPRNG_DISTRIBUTION_UNIT_EXPONENTIAL_HPP
