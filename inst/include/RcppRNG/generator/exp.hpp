#ifndef RCPPRNG_GENERATOR_EXP_HPP
#define RCPPRNG_GENERATOR_EXP_HPP

#include <RcppRNG/generator/generator.hpp>

namespace RcppRNG {

template<typename T>
class ExpGenerator : public Generator<T, double> {
public:
  ExpGenerator();
  ExpGenerator(double rate = 1.);

  inline double operator()() const;
private:
  double scale_;
  const T rng_;
}; // ExpGenerator<T, double>

template<typename T>
ExpGenerator<T>::ExpGenerator() :
  scale_(1.) {}

template<typename T>
ExpGenerator<T>::ExpGenerator(double rate) :
  scale_(1. / rate) {}

} // RcppRNG

#endif // RCPPRNG_GENERATOR_EXP_HPP
