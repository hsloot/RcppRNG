#ifndef RCPPRNG_GENERATOR_GENERATOR_HPP
#define RCPPRNG_GENERATOR_GENERATOR_HPP

namespace RcppRNG {

template<typename T, typename S>
class Generator {
public:
  Generator() {}
  virtual ~Generator() {}

  virtual inline S operator()() const = 0;
private:
  const T rng_;
}; // Generator<T, S>

} // RcppRNG

#endif // RCPPRNG_GENERATOR_GENERATOR_HPP
