#ifndef RCPPRNG_MISC_OBJECTCOUNTER_HPP
#define RCPPRNG_MISC_OBJECTCOUNTER_HPP

namespace RcppRNG {

// see https://stackoverflow.com/a/185848/11109217
// TODO: Move into separate file and also use it for
// the other RNG classes.
template<typename T>
class ObjectCounter {
public:
  ObjectCounter();
  ObjectCounter(const ObjectCounter& obj);
  ~ObjectCounter();

  static size_t OutstandingObjects();

private:
  static size_t totalObjects_;
};

template<typename T>
ObjectCounter<T>::ObjectCounter() {
  ++totalObjects_;
}

template<typename T>
ObjectCounter<T>::ObjectCounter(const ObjectCounter<T>& obj) {
  if(this != &obj)
    ++totalObjects_;
}

template<typename T>
ObjectCounter<T>::~ObjectCounter() {
  --totalObjects_;
}

template<typename T>
size_t ObjectCounter<T>::OutstandingObjects() {
  return totalObjects_;
}

}

#endif // RCPPRNG_MISC_OBJECTCOUNTER_HPP
