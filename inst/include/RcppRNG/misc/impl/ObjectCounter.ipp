#ifndef RCPPRNG_MISC_OBJECTCOUNTER_IPP
#define RCPPRNG_MISC_OBJECTCOUNTER_IPP

#include <RcppRNG/misc/ObjectCounter.hpp>

namespace RcppRNG {

namespace misc {

template <typename _Tp>
ObjectCounter<_Tp>::ObjectCounter() {
  ++total_;
}

template <typename _Tp>
ObjectCounter<_Tp>::ObjectCounter(
    const ObjectCounter<_Tp>& other) {
  if (this != &other) ++total_;
}

template <typename _Tp>
ObjectCounter<_Tp>::ObjectCounter(ObjectCounter<_Tp>&& other) noexcept = default;

template <typename _Tp>
ObjectCounter<_Tp>::~ObjectCounter() {
  --total_;
}

template <typename _Tp>
ObjectCounter<_Tp>& ObjectCounter<_Tp>::operator=(
    const ObjectCounter<_Tp>& other) {
  ++total_;
}

template <typename _Tp>
ObjectCounter<_Tp>& ObjectCounter<_Tp>::operator=(ObjectCounter<_Tp>&& other) noexcept = default;

template <typename _Tp>
auto ObjectCounter<_Tp>::OutstandingObjects() {
  return total_;
}

}  // namespace misc

}  // namespace RcppRNG

#endif  // RCPPRNG_MISC_OBJECTCOUNTER_IPP
