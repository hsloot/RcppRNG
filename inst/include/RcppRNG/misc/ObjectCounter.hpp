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
  ObjectCounter(const ObjectCounter& other);
  ObjectCounter(ObjectCounter&& other) = default; //move constructor
  ~ObjectCounter();

  ObjectCounter& operator = (const ObjectCounter &other);
  ObjectCounter& operator=(ObjectCounter&& other) = default;

  static size_t OutstandingObjects();

private:
  static size_t totalObjects_;
}; // Object Counter

template<typename T>
ObjectCounter<T>::ObjectCounter() {
  ++totalObjects_;
}

template<typename T>
ObjectCounter<T>::ObjectCounter(const ObjectCounter<T>& other) {
  if(this != &other)
    ++totalObjects_;
}

template<typename T>
ObjectCounter<T>::~ObjectCounter() {
  --totalObjects_;
}

template<typename T>
ObjectCounter<T>& ObjectCounter<T>::operator = (const ObjectCounter<T> &other) {
  ++totalObjects_;
}

template<typename T>
size_t ObjectCounter<T>::OutstandingObjects() {
  return totalObjects_;
}

} // RcppRNG

#endif // RCPPRNG_MISC_OBJECTCOUNTER_HPP
