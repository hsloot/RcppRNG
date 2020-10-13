#ifndef RCPPRNG_DISTRIBUTION_UNIT_EXPONENTIAL_HPP
#define RCPPRNG_DISTRIBUTION_UNIT_EXPONENTIAL_HPP

#include <limits>
#include <type_traits>

namespace RcppRNG {

namespace distribution {

template <typename _RealType = double>
class unit_exponential_distribution {
 public:
  using result_type = _RealType;

  static_assert(std::is_floating_point<_RealType>::value,
                "_RealType not floating point number");
  static_assert(std::numeric_limits<_RealType>::is_iec559,
                "_RealType not IEEE 754");

  // default ctor's and copy assignment operator are sufficient.

  template <typename _EngineType>
  result_type operator()(_EngineType& eng);
};

}  // namespace distribution

}  // namespace RcppRNG

#endif  // RCPPRNG_DISTRIBUTION_UNIT_EXPONENTIAL_HPP
