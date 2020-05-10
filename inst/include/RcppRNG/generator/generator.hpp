#ifndef RCPPRNG_GENERATOR_GENERATOR_HPP
#define RCPPRNG_GENERATOR_GENERATOR_HPP

namespace RcppRNG {

template<typename T, typename S>
class Generator {
public:
  Generator() {}
  ~Generator() {}

  virtual inline S operator()() const = 0;
private:
  const T rng;
}; // Generator<T, S>

} // RcppRNG

#endif // RCPPRNG_GENERATOR_GENERATOR_HPP
