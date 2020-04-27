#ifndef RCPPRNG_RNG_DQRNG_HPP
#define RCPPRNG_RNG_DQRNG_HPP

#include <RcppRNG/RNG/RNG.hpp>
#include <Rcpp.h>
#include <dqrng_generator.h>
#include <xoshiro.h>
#include <R_randgen.h>
#include <convert_seed.h>


namespace RcppRNG {

// see https://stackoverflow.com/a/185848/11109217
// TODO: Move into separate file and also use it for
// the other RNG classes.
template<class Obj>
class CountedObj {
public:
  CountedObj() {++total_;}
  CountedObj(const CountedObj& obj) {if(this != &obj) ++total_;}
  ~CountedObj() {--total_;}

  static size_t OustandingObjects() {return total_;}

private:
  static size_t total_;
};


class DQRNG : public RNG, private CountedObj<DQRNG> {
public:
  DQRNG();
  ~DQRNG();

  static dqrng::rng64_t shared_rng;
private:
  static size_t total_;
};

#ifndef RCPPRNG_TOTAL
#define RCPPRNG_TOTAL
template<>
size_t CountedObj<DQRNG>::total_ = 0;
#endif

DQRNG::DQRNG() {
  if (this->OustandingObjects() == 1) {
    GetRNGstate();
    Rcpp::IntegerVector seed(2, dqrng::R_random_int);
    shared_rng = dqrng::generator(dqrng::convert_seed<uint64_t>(seed));
  }
}

DQRNG::~DQRNG() {
  if (this->OustandingObjects() == 1) {
    PutRNGstate();
  }
}

dqrng::rng64_t DQRNG::shared_rng;

} // RcppRNG

#endif // RCPPRNG_RNG_DQRNG_HPP