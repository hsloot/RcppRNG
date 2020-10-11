#ifndef RCPPRNG_RNG_RCPPRNG_HPP
#define RCPPRNG_RNG_RCPPRNG_HPP

#include <RcppRNG/RNG/RNG.hpp>
#include <RcppRNG/misc/ObjectCounter.hpp>
#include <R_ext/Random.h>

namespace RcppRNG {

namespace rng {

class RcppRNG : public RNG, private misc::ObjectCounter<RcppRNG> {
 public:
  RcppRNG();
  ~RcppRNG();
};  // RcppRNG

} // namespace rng

#ifndef RCPPRNG_RCPPRNG_TOTAL
#define RCPPRNG_RCPPRNG_TOTAL
template <>
std::size_t misc::ObjectCounter<rng::RcppRNG>::total_ = std::size_t{0};
#endif  // RCPPRNG_RCPPRNG_TOTAL

namespace rng {

  RcppRNG::RcppRNG() {
    if (1 == this->OutstandingObjects()) GetRNGstate();
  }

  RcppRNG::~RcppRNG() {
    if (1 == this->OutstandingObjects()) PutRNGstate();
  }

} // namespace rng


}  // namespace RcppRNG

#endif  // RCPPRNG_RNG_RCPPRNG_HPP
