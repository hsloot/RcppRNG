#ifndef RCPPRNG_GENERATOR_EXP_HPP
#define RCPPRNG_GENERATOR_EXP_HPP

#include <RcppRNG/distribution/exp.hpp>
#include <RcppRNG/generator/generator.hpp>

namespace RcppRNG {

template <typename T>
using ExpGenerator = Generator<T, double, ExpDistribution<double>>;

}  // namespace RcppRNG

#endif  // RCPPRNG_GENERATOR_EXP_HPP
