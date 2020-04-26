#ifndef RCPPRNG_RNG_RCPPRNG_HPP
#define RCPPRNG_RNG_RCPPRNG_HPP

namespace RcppRNG {

class RNG {
public:
  virtual ~RNG() {};
};

class RcppRNG : public RNG {
public:
  RcppRNG();
  ~RcppRNG();
private:
  static int rngSynchronizationSuspended;
};

RcppRNG::RcppRNG() {
  if (0 == this->rngSynchronizationSuspended++)
    GetRNGstate();
}

RcppRNG::~RcppRNG() {
  if (0 == --this->rngSynchronizationSuspended)
    PutRNGstate();
}

int RcppRNG::rngSynchronizationSuspended = 0;

} // RcppRNG

#endif // RCPPRNG_RNG_RCPPRNG_HPP