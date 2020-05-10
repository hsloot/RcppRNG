#ifndef RCPPRNG_MISC_OBJECTCOUNTER_HPP
#define RCPPRNG_MISC_OBJECTCOUNTER_HPP

namespace RcppRNG {

// see https://stackoverflow.com/a/185848/11109217
// TODO: Move into separate file and also use it for
// the other RNG classes.
template<typename C>
class ObjectCounter {
public:
  ObjectCounter() {++totalObjects_;}
  ObjectCounter(const ObjectCounter& obj) {if(this != &obj) ++totalObjects_;}
  ~ObjectCounter() {--totalObjects_;}

  static size_t OustandingObjects() {return totalObjects_;}

private:
  static size_t totalObjects_;
};

}

#endif // RCPPRNG_MISC_OBJECTCOUNTER_HPP
