#ifndef RCPPRNG_MISC_OBJECTCOUNTER_HPP
#define RCPPRNG_MISC_OBJECTCOUNTER_HPP

#include <cstddef>

namespace RcppRNG {

namespace misc {

// see https://stackoverflow.com/a/185848/11109217
// TODO: Move into separate file and also use it for
// the other RNG classes.
template <typename _Tp>
class ObjectCounter {
 public:
  ObjectCounter() {
    ++total_;
  }
  ObjectCounter(
      const ObjectCounter<_Tp>& other) {
    if (this != &other) ++total_;
  }
  ObjectCounter(ObjectCounter&& other) noexcept = default;

  ~ObjectCounter() {
    --total_;
  }

  ObjectCounter& operator=(
      const ObjectCounter<_Tp>& other) {
    ++total_;
  }
  ObjectCounter& operator=(ObjectCounter&& other) noexcept = default;

  static std::size_t OutstandingObjects() {
    return total_;
  }

 private:
  static std::size_t total_;
};  // class ObjectCounter

} // namespace misc

}  // namespace RcppRNG

#endif  // RCPPRNG_MISC_OBJECTCOUNTER_HPP
