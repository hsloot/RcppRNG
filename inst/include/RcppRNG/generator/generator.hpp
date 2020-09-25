#ifndef RCPPRNG_GENERATOR_GENERATOR_HPP
#define RCPPRNG_GENERATOR_GENERATOR_HPP

namespace RcppRNG {

template <typename T, typename S, typename U>
class Generator {
 public:
  Generator() {}
  Generator(const U &param) : param_(param) {}
  ~Generator() {}

  inline S operator()() const;

 private:
  const T rng_;
  const U param_;
};  // Generator<T, S>

}  // namespace RcppRNG

#endif  // RCPPRNG_GENERATOR_GENERATOR_HPP
