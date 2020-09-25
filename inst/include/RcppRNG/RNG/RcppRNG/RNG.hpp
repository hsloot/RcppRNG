#ifndef RCPPRNG_RNG_RCPPRNG_HPP
#define RCPPRNG_RNG_RCPPRNG_HPP

#include <R_ext/Random.h>
#include <RcppRNG/RNG/RNG.hpp>
#include <RcppRNG/misc/ObjectCounter.hpp>

namespace RcppRNG {

class RcppRNG : public RNG, private ObjectCounter<RcppRNG> {
 public:
  RcppRNG();
  ~RcppRNG();
};  // RcppRNG

#ifndef RCPPRNG_RCPPRNG_TOTAL
#define RCPPRNG_RCPPRNG_TOTAL
template <>
size_t ObjectCounter<RcppRNG>::totalObjects_ = 0;
#endif  // RCPPRNG_RCPPRNG_TOTAL

RcppRNG::RcppRNG() {
  if (1 == this->OutstandingObjects()) GetRNGstate();
}

RcppRNG::~RcppRNG() {
  if (1 == this->OutstandingObjects()) PutRNGstate();
}

}  // namespace RcppRNG

#endif  // RCPPRNG_RNG_RCPPRNG_HPP
