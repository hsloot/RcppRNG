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
  ObjectCounter();
  ObjectCounter(const ObjectCounter& other);
  ObjectCounter(ObjectCounter&& other) noexcept;

  ~ObjectCounter();

  ObjectCounter& operator=(const ObjectCounter& other);
  ObjectCounter& operator=(ObjectCounter&& other) noexcept ;

  static auto OutstandingObjects();

 private:
  static std::size_t total_;
};  // class ObjectCounter

} // namespace misc

}  // namespace RcppRNG

#include <RcppRNG/misc/impl/ObjectCounter.ipp>

#endif  // RCPPRNG_MISC_OBJECTCOUNTER_HPP
