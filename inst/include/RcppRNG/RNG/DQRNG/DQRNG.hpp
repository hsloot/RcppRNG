#ifndef RCPPRNG_RNG_DQRNG_HPP
#define RCPPRNG_RNG_DQRNG_HPP

#include <R_randgen.h>
#include <Rcpp.h>
#include <convert_seed.h>
#include <dqrng_generator.h>
#include <dqrng_distribution.h>
#include <xoshiro.h>

#include <RcppRNG/RNG/RNG.hpp>
#include <RcppRNG/misc/ObjectCounter.hpp>

namespace RcppRNG {

namespace rng {

class DQRNG : public RNG, private misc::ObjectCounter<DQRNG> {
 public:
  DQRNG();
  ~DQRNG();

  static dqrng::rng64_t shared_rng;
  static const dqrng::exponential_distribution ued;
};  // DQRNG

const dqrng::exponential_distribution DQRNG::ued{};

}  // namespace rng

#ifndef RCPPRNG_DQRNG_TOTAL
#define RCPPRNG_DQRNG_TOTAL
template <>
std::size_t misc::ObjectCounter<rng::DQRNG>::total_ = std::size_t{0};
#endif  // RCPPRNG_DQRNG_TOTAL

namespace rng {

DQRNG::DQRNG() {
  if (this->OutstandingObjects() == 1) {
    GetRNGstate();
    Rcpp::IntegerVector seed(2, dqrng::R_random_int);
    shared_rng = dqrng::generator(dqrng::convert_seed<uint64_t>(seed));
  }
}

DQRNG::~DQRNG() {
  if (this->OutstandingObjects() == 1) {
    // no call to PutRNGstate() required?!
  }
}

dqrng::rng64_t DQRNG::shared_rng;

}  // namespace rng

}  // namespace RcppRNG

#endif  // RCPPRNG_RNG_DQRNG_HPP
