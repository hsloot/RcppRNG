#ifndef RCPPRNG_DISTRIBUTION_EXP_HPP
#define RCPPRNG_DISTRIBUTION_EXP_HPP

#include <limits>
#include <stdexcept>

#include <RcppRNG/distribution/unit_exponential_distribution.hpp>

namespace RcppRNG {

namespace distribution {

template <typename _RealType = double,
          typename _UnitExponentialDistributionType =
              unit_exponential_distribution<_RealType>>
class exponential_distribution {
 public:
  using input_type = _RealType;
  using output_type = _RealType;

  class param_type {
   public:
    using distribution_type = exponential_distribution;

    param_type(_RealType lambda = _RealType(1.))
        : lambda_{lambda}, unit_exponential_distribution_{} {
      check_params();
    }

    // compiler generated ctor and assignment op is sufficient

    static _RealType min() { return _RealType(0.); }

    static _RealType max() {
      return std::numeric_limits<_RealType>::infinity();
    }

    _RealType lambda() const { return lambda_; }

    template <typename _EngineType>
    typename _UnitExponentialDistributionType::output_type
    unit_exponential_distribution(_EngineType&& engine) {
      return unit_exponential_distribution_(engine);
    }

   private:
    _RealType lambda_;
    _UnitExponentialDistributionType unit_exponential_distribution_;

    void check_params() {
      if (lambda_ < min() || lambda_ > max())
        throw std::range_error("lambda out of range");
    }
  };

  explicit exponential_distribution(_RealType rate = _RealType(1.))
      : params_{rate} {}

  explicit exponential_distribution(const param_type& params)
      : params_(params) {}

  // compiler generated ctor and assignment op is sufficient

  template <typename _EngineType>
  _RealType operator()(_EngineType&& engine) {
    return (*this)(params_.lambda(), std::forward<_EngineType>(engine));
  }

  template <typename _EngineType>
  _RealType operator()(const _RealType& lambda, _EngineType&& engine) {
    return params_.unit_exponential_distribution(
               std::forward<_EngineType>(engine)) /
           lambda;
  }

  friend bool operator==(const exponential_distribution& lhs,
                         const exponential_distribution& rhs);

  friend bool operator!=(const exponential_distribution& lhs,
                         const exponential_distribution& rhs);

 private:
  param_type params_;
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
  return lhs.isEqual(rhs) && lhs.params_ != rhs.params_;
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
