#include <Rcpp.h>

template <typename T>
class ObjectCounter {
 public:
  ObjectCounter();
  ObjectCounter(const ObjectCounter& other);
  ObjectCounter(ObjectCounter&& other) = default;  // move constructor
  ~ObjectCounter();

  ObjectCounter& operator=(const ObjectCounter& other);
  ObjectCounter& operator=(ObjectCounter&& other) = default;

  static size_t OutstandingObjects();

 private:
  static size_t totalObjects_;
};  // Object Counter

template <typename T>
ObjectCounter<T>::ObjectCounter() {
  ++totalObjects_;
}

template <typename T>
ObjectCounter<T>::ObjectCounter(const ObjectCounter<T>& other) {
  if (this != &other) ++totalObjects_;
}

template <typename T>
ObjectCounter<T>::~ObjectCounter() {
  --totalObjects_;
}

template <typename T>
ObjectCounter<T>& ObjectCounter<T>::operator=(const ObjectCounter<T>& other) {
  ++totalObjects_;
}

template <typename T>
size_t ObjectCounter<T>::OutstandingObjects() {
  return totalObjects_;
}

class RNG {
 public:
  virtual ~RNG() = default;
};  // RNG

template <typename _RNG>
using is_rng = std::is_base_of<RNG, _RNG>;

class RcppRNG : public RNG, private ObjectCounter<RcppRNG> {
 public:
  RcppRNG();
  ~RcppRNG();
};  // RcppRNG

template <>
size_t ObjectCounter<RcppRNG>::totalObjects_ = 0;

RcppRNG::RcppRNG() {
  if (1 == this->OutstandingObjects()) GetRNGstate();
}

RcppRNG::~RcppRNG() {
  if (1 == this->OutstandingObjects()) PutRNGstate();
}

static_assert(is_rng<RcppRNG>::value, "RcppRNG is not derived from RNG");

class Generator {};

template <typename _Generator>
using is_generator = std::is_base_of<Generator, _Generator>;

class Uniform01Generator : public Generator {};

template <typename _Generator>
using is_uniform_generator = std::is_base_of<Uniform01Generator, _Generator>;

template <typename RealType>
class Uniform01GeneratorR : public Uniform01Generator {
 public:
  Uniform01GeneratorR() = default;
  Uniform01GeneratorR(const Uniform01GeneratorR&) = default;
  Uniform01GeneratorR(Uniform01GeneratorR&&) = default;

  ~Uniform01GeneratorR() = default;

  Uniform01GeneratorR& operator=(const Uniform01GeneratorR&) = default;
  Uniform01GeneratorR& operator=(Uniform01GeneratorR&&) = default;

  RealType operator()();

 private:
  RcppRNG rng_;
};

template <typename RealType>
inline RealType Uniform01GeneratorR<RealType>::operator()() {
  static_assert(std::is_convertible<double, RealType>::value,
                "double not convertible to RealType");
  return ::unif_rand();
}

static_assert(is_generator<Uniform01GeneratorR<double>>::value,
              "Uniform01GeneratorR<double> is not a Generator");

template <typename RealType, typename Uniform01Generator>
class UniformNumberGenerator : public Generator {
 public:
  UniformNumberGenerator() = default;
  UniformNumberGenerator(const UniformNumberGenerator&) = default;
  UniformNumberGenerator(UniformNumberGenerator&&) = default;

  UniformNumberGenerator(const RealType size);

  ~UniformNumberGenerator() = default;

  UniformNumberGenerator& operator=(const UniformNumberGenerator&) = default;
  UniformNumberGenerator& operator=(UniformNumberGenerator&&) = default;

  RealType operator()();

 private:
  RealType size_ = 1;
  static_assert(is_uniform_generator<Uniform01Generator>::value,
                "Uniform01Generator not a Uniform01Generator");
  Uniform01Generator unif_gen_;
};

template <typename RealType, typename Uniform01Generator>
UniformNumberGenerator<RealType, Uniform01Generator>::UniformNumberGenerator(
    const RealType size)
    : size_{size}, unif_gen_{} {}

template <typename RealType, typename Uniform01Generator>
RealType UniformNumberGenerator<RealType, Uniform01Generator>::operator()() {
  static_assert(std::is_convertible<double, RealType>::value,
                "double not convertible to RealType");
  return floor(size_ * unif_gen_());
}

static_assert(
    is_generator<
        UniformNumberGenerator<double, Uniform01GeneratorR<double>>>::value,
    "UniformNumberGenerator<double, Uniform01GeneratorR<double>> is not a "
    "Generator");

template <typename RealType>
class UniformIndexGeneratorR : public Generator {
 public:
  UniformIndexGeneratorR() = default;
  UniformIndexGeneratorR(const UniformIndexGeneratorR&) = default;
  UniformIndexGeneratorR(UniformIndexGeneratorR&&) = default;

  UniformIndexGeneratorR(const RealType size);

  ~UniformIndexGeneratorR() = default;

  UniformIndexGeneratorR& operator=(const UniformIndexGeneratorR&) = default;
  UniformIndexGeneratorR& operator=(UniformIndexGeneratorR&&) = default;

  RealType operator()();

 private:
  RealType size_ = 1;
  RcppRNG rng_;
};

template <typename RealType>
UniformIndexGeneratorR<RealType>::UniformIndexGeneratorR(const RealType size)
    : size_{size}, rng_{} {}

template <typename RealType>
RealType UniformIndexGeneratorR<RealType>::operator()() {
  static_assert(std::is_convertible<double, RealType>::value,
                "double not convertible to RealType");
  return ::R_unif_index(size_);
}

static_assert(is_generator<UniformIndexGeneratorR<std::size_t>>::value,
              "UniformIndexGeneratorR<std::size_t> is not a Generator");

using namespace Rcpp;

// [[Rcpp::export]]
NumericVector r_sample(const std::size_t n, const std::size_t size) {
  UniformNumberGenerator<double, Uniform01GeneratorR<double>> generator(size);
  NumericVector out(no_init(n));
  std::generate(out.begin(), out.end(), generator);

  return out;
}

// [[Rcpp::export]]
NumericVector r_index(const std::size_t n, const std::size_t size) {
  UniformIndexGeneratorR<double> generator(size);
  NumericVector out(no_init(n));
  std::generate(out.begin(), out.end(), generator);

  return out;
}

/*** R
set.seed(1263L)
floor(runif(10L) * 20)
floor(runif(10L) * 20)

set.seed(1263L)
r_sample(10L, 20)
r_sample(10L, 20)

set.seed(1263L)
sample.int(20, 10, replace=TRUE)-1
sample.int(20, 10, replace=TRUE)-1

set.seed(1263L)
r_index(10L, 20)
r_index(10L, 20)
*/
