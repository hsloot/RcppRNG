#ifndef RCPPRNG_DISTRIBUTION_EXP_HPP
#define RCPPRNG_DISTRIBUTION_EXP_HPP

#include <limits>
#include <stdexcept>

#include <RcppRNG/distribution/unit_exponential_distribution.hpp>

namespace RcppRNG {

namespace distribution {

namespace exponential {

template <typename _RealType>
void check_params(const _RealType lambda) {
  if (!(0. <= lambda && lambda <= std::numeric_limits<_RealType>::infinity()))
    throw std::range_error("lambda not in [0, infty]");
}

}  // namespace exponential

template <typename _RealType = double,
          typename _UnitExponentialDistributionType =
              unit_exponential_distribution<_RealType>>
class exponential_distribution {
 public:
  using result_type = _RealType;
  static_assert(std::is_floating_point<_RealType>::value,
                "_RealType not floating point number");
  static_assert(std::numeric_limits<_RealType>::is_iec559,
                "_RealType not IEEE 754");

  class param_type {
   public:
    using distribution_type = exponential_distribution;

    param_type(_RealType lambda = _RealType{1.}) : lambda_{lambda} {
      exponential::check_params(lambda);
    }

    // compiler generated ctor and assignment op is sufficient

    _RealType lambda() const { return lambda_; }

   private:
    _RealType lambda_;
  };

  explicit exponential_distribution(_RealType lambda = _RealType{1.})
      : lambda_{lambda}, unit_exponential_distribution_{} {
    exponential::check_params(lambda_);
  }

  explicit exponential_distribution(const param_type& params)
      : lambda_{params.lambda()}, unit_exponential_distribution_{} {}

  // compiler generated ctor and assignment op is sufficient

  static _RealType min() { return _RealType{0.}; }

  static _RealType max() { return std::numeric_limits<_RealType>::infinity(); }

  param_type params() const { return param_type{lambda_}; }

  void params(const param_type& params) { lambda_ = params.lambda(); }

  _RealType lambda() const { return lambda_; }

  void reset() {}

  template <typename _EngineType>
  result_type operator()(_EngineType&& engine) {
    return unit_exponential_distribution_(std::forward<_EngineType>(engine)) /
           lambda_;
  }

  template <typename _EngineType>
  result_type operator()(_EngineType&& engine, const param_type& params) {
    return typename param_type::distribution_type{params}(
        std::forward<_EngineType>(engine));
  }

 private:
  _RealType lambda_;
  _UnitExponentialDistributionType unit_exponential_distribution_;
};  // exponential_distribution

template <typename _RealType, typename _UnitExponentialDistributionType>
bool operator==(
    const typename exponential_distribution<
        _RealType, _UnitExponentialDistributionType>::param_type& lhs,
    const typename exponential_distribution<
        _RealType, _UnitExponentialDistributionType>::param_type& rhs) {
  return lhs.isEqual(rhs) && lhs.lambda() == rhs.lambda();
}

template <typename _RealType, typename _UnitExponentialDistributionType>
bool operator!=(
    const typename exponential_distribution<
        _RealType, _UnitExponentialDistributionType>::param_type& lhs,
    const typename exponential_distribution<
        _RealType, _UnitExponentialDistributionType>::param_type& rhs) {
  return !(lhs == rhs);
}

template <typename _RealType, typename _UnitExponentialDistributionType>
bool operator==(
    const exponential_distribution<_RealType, _UnitExponentialDistributionType>&
        lhs,
    const exponential_distribution<_RealType, _UnitExponentialDistributionType>&
        rhs) {
  return lhs.isEqual(rhs) && lhs.params() != rhs.params();
}

template <typename _RealType, typename _UnitExponentialDistributionType>
bool operator!=(
    const exponential_distribution<_RealType, _UnitExponentialDistributionType>&
        lhs,
    const exponential_distribution<_RealType, _UnitExponentialDistributionType>&
        rhs) {
  return !(lhs == rhs);
}

}  // namespace distribution

}  // namespace RcppRNG

#endif  // RCPPRNG_DISTRIBUTION_EXP_HPP
