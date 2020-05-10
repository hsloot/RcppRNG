
<!-- README.md is generated from README.Rmd. Please edit that file -->

# RcppRNG

<!-- badges: start -->

[![R build
status](https://github.com/hsloot/RcppRNG/workflows/R-CMD-check/badge.svg)](https://github.com/hsloot/RcppRNG/actions)
[![Codecov test
coverage](https://codecov.io/gh/hsloot/RcppRNG/branch/master/graph/badge.svg)](https://codecov.io/gh/hsloot/RcppRNG?branch=master)
![lint](https://github.com/hsloot/RcppRNG/workflows/lint/badge.svg)
[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://www.tidyverse.org/lifecycle/#experimental)
<!-- badges: end -->

The goal of RcppRNG is to explore a templated implementation of Rcpp’s
internal generators for sampling. The motivation is to be able to write
sampling routines that are based on native sampling routines (`rexp`,
`runif`, …) in a way that one can easily switch to an alternative RNG
(e.g. `dqrng`).

## Installation

Warning: This package is currently only indented as a showcase. If you
want to replicate the benchmarks in this package, you can install it via

``` r
remotes::install_github("hsloot/RcppRNG")
```

## Demonstration

At the moment, only a templated version of the generator for the
exponential function is implemented. The generator template allows
variations for difference RNG classes (which are essentially assumed to
behave like `Rcpp::RNGScope`). I implemented my own version of
`Rcpp::RNGScope` in `RcppRNG::RcppRNG` which contains it’s own static
counter instead going through R’s Call API. Both version seem to be very
similar in their performance.

``` cpp
#include <Rcpp.h>

// [[Rcpp::plugins("cpp11")]]
// [[Rcpp::depends(RcppRNG)]]
// [[Rcpp::depends(dqrng)]]
// [[Rcpp::depends(BH)]]
// [[Rcpp::depends(sitmo)]]

#include <RcppRNG.hpp>

using namespace Rcpp;

// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rexp_RNGScope(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  RcppRNG::ExpGenerator<RNGScope> gen(rate);
  std::generate(out.begin(), out.end(), gen);

  return out;
}

// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rexp_RcppRNG(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  RcppRNG::ExpGenerator<RcppRNG::RcppRNG> gen(rate);
  std::generate(out.begin(), out.end(), gen);

  return out;
}
```

``` r
use_seed <- 1623L

set.seed(use_seed)
rexp(10, 0.5)
#>  [1] 5.4676605 4.5936452 0.4407928 0.5377660 5.1456763 1.4054108 1.6753103
#>  [8] 3.2669849 0.1426958 4.6437262

set.seed(use_seed)
Rcpp_rexp_RNGScope(10, 0.5)
#>  [1] 5.4676605 4.5936452 0.4407928 0.5377660 5.1456763 1.4054108 1.6753103
#>  [8] 3.2669849 0.1426958 4.6437262

set.seed(use_seed)
Rcpp_rexp_RcppRNG(10, 0.5)
#>  [1] 5.4676605 4.5936452 0.4407928 0.5377660 5.1456763 1.4054108 1.6753103
#>  [8] 3.2669849 0.1426958 4.6437262

bench::mark(
  rexp(1e5, 0.5),
  Rcpp_rexp_RNGScope(1e5, 0.5), 
  Rcpp_rexp_RcppRNG(1e5, 0.5), 
  check=FALSE
)
#> # A tibble: 3 x 6
#>   expression                          min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr>                     <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 rexp(1e+05, 0.5)                 5.42ms   6.33ms      152.  783.79KB     2.05
#> 2 Rcpp_rexp_RNGScope(1e+05, 0.5)   3.85ms   4.42ms      218.    4.52MB     2.05
#> 3 Rcpp_rexp_RcppRNG(1e+05, 0.5)    4.02ms   5.81ms      167.  787.92KB     4.24
```

## Why is that useful?

For the exponential distribution, the problem is not that easy to
motivate, since whenever one wants repeated single exponential samples
inside the C++ function, one could resort to

``` cpp
scale * R::exp_rand();
```

However, it is much more difficult, if you need repeated single samples
from a discrete distribution (e.g. to sample the transitions in a Markov
process). At his point you could do somethings like

``` cpp
sample(n, 1, false, probs, true)[0]; // probs is NumericVector of size n
```

This, however, would construct a `NumericVector` object in each draw.
This has a significant overhead. For the exponential distribution this
will be demonstrated in the following

``` cpp
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector Rcpp_rexp_slow(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  for (R_xlen_t k=0; k<n; k++) 
    out[k] = rexp(1, rate)[0];

  return out;
}
```

``` r
bench::mark(
  rexp(1e5, 0.5),
  Rcpp_rexp_RNGScope(1e5, 0.5), 
  Rcpp_rexp_RcppRNG(1e5, 0.5), 
  Rcpp_rexp_slow(1e5, 0.5),
  check=FALSE
)
#> # A tibble: 4 x 6
#>   expression                          min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr>                     <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 rexp(1e+05, 0.5)                 5.75ms   6.78ms     144.      784KB     2.05
#> 2 Rcpp_rexp_RNGScope(1e+05, 0.5)   3.95ms   4.53ms     207.      791KB     2.07
#> 3 Rcpp_rexp_RcppRNG(1e+05, 0.5)    3.85ms   4.32ms     226.      784KB     4.22
#> 4 Rcpp_rexp_slow(1e+05, 0.5)       8.62ms  10.18ms      98.2     784KB    40.6
```

However, we can also use another random number generator (e.g. the one
from `dqrng`):

``` cpp
#include <Rcpp.h>

// [[Rcpp::plugins("cpp11")]]
// [[Rcpp::depends(RcppRNG)]]
// [[Rcpp::depends(dqrng)]]
// [[Rcpp::depends(BH)]]
// [[Rcpp::depends(sitmo)]]

#include <RcppRNG.hpp>

using namespace Rcpp;

RcppRNG::DQRNG shared_dqrng = RcppRNG::DQRNG();  

// [[Rcpp::export(rng=false)]]
void dqset_seed2(Rcpp::IntegerVector seed, Rcpp::Nullable<Rcpp::IntegerVector> stream = R_NilValue) {
  uint64_t _seed = dqrng::convert_seed<uint64_t>(seed);
  if (stream.isNotNull()) {
    uint64_t _stream = dqrng::convert_seed<uint64_t>(stream.as());
    shared_dqrng.shared_rng->seed(_seed, _stream);
  } else {
    shared_dqrng.shared_rng->seed(_seed);
  }
}

// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rexp_DQRNG(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  RcppRNG::ExpGenerator<RcppRNG::DQRNG> gen(rate);
  std::generate(out.begin(), out.end(), gen);

  return out;
}
```

``` r
dqset_seed2(use_seed)
Rcpp_rexp_DQRNG(1e1, 0.5) 
#>  [1] 0.3163569 2.0283718 1.2877356 1.9899493 0.1225778 9.9800737 1.1667144
#>  [8] 2.2395507 0.6990022 0.1845893

dqset_seed2(use_seed)
Rcpp_rexp_DQRNG(1e1, 0.5)
#>  [1] 0.3163569 2.0283718 1.2877356 1.9899493 0.1225778 9.9800737 1.1667144
#>  [8] 2.2395507 0.6990022 0.1845893


bench::mark(
  rexp(1e5, 0.5),
  Rcpp_rexp_RNGScope(1e5, 0.5),
  Rcpp_rexp_RcppRNG(1e5, 0.5),
  Rcpp_rexp_DQRNG(1e5, 0.5),
  Rcpp_rexp_slow(1e5, 0.5),
  check=FALSE
)
#> # A tibble: 5 x 6
#>   expression                          min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr>                     <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 rexp(1e+05, 0.5)                 5.45ms   6.58ms     148.      784KB     2.06
#> 2 Rcpp_rexp_RNGScope(1e+05, 0.5)   3.84ms   4.89ms     199.      791KB     4.28
#> 3 Rcpp_rexp_RcppRNG(1e+05, 0.5)    3.86ms   4.55ms     207.      784KB     2.05
#> 4 Rcpp_rexp_DQRNG(1e+05, 0.5)    833.11µs   1.26ms     749.      781KB    13.8 
#> 5 Rcpp_rexp_slow(1e+05, 0.5)       8.34ms   9.64ms      98.2     784KB    42.6
```

The main benefit of this design is that it allows us to implement new
sampling algorithms, which are based on basic generators (e.g. exp,
unif, …) such that they can be used with the base R RNG or other RNG’s
(e.g. dqrng’s):

``` cpp
// example.hpp
#include <Rcpp.h>

// [[Rcpp::plugins("cpp11")]]
// [[Rcpp::depends(RcppRNG)]]
// [[Rcpp::depends(dqrng)]]
// [[Rcpp::depends(BH)]]
// [[Rcpp::depends(sitmo)]]

#include <RcppRNG.hpp>

using namespace Rcpp;

template<typename T>
class ErlangGenerator : public Generator<T, double> {
public:
  ErlangGenerator();
  ErlangGenerator(int shape = 1, double rate = 1.);

  inline double operator()() const;
private:
  int shape_;
  double rate_;
  const T rng;
};

template<typename T>
ErlangGenerator<T>::ErlangGenerator() :
  scale_(1.) {}

template<typename T>
ErlangGenerator<T>::ErlangGenerator(int shape, double rate) :
  shape_(shape), rate_(rate) {}

template<typename T>
inline double ErlangGenerator<T>::operator()() const {
  double out = 0.;
  ExpGenerator<T> exp(rate);
  for (int j=1; j<=shape; j++)
    out += exp();
  
  return out;
} 
```

``` cpp
// example.cpp
#include <Rcpp.h>

// [[Rcpp::plugins("cpp11")]]
// [[Rcpp::depends(RcppRNG)]]
// [[Rcpp::depends(dqrng)]]
// [[Rcpp::depends(BH)]]
// [[Rcpp::depends(sitmo)]]

#include <RcppRNG.hpp>
#include "example.hpp"

using namespace Rcpp;

//' @export
// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rerlang_RcppRNG(R_xlen_t n, int shape = 1, double rate = 1.) {
  NumericVector out(no_init(n));
  ErlangGenerator<RcppRNG::RcppRNG> gen(shape, rate);
  std::generate(out.begin(), out.end(), gen); 

  return out;
}

//' @export
// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rerlang_DQRNG(R_xlen_t n, int shape = 1, double rate = 1.) {
  NumericVector out(no_init(n));
  ErlangGenerator<RcppRNG::DQRNG> gen(shape, rate);
  std::generate(out.begin(), out.end(), gen); 

  return out;
}
/*** R
Rcpp_rerlang_RcppRNG(10, 3, 0.5)
Rcpp_rerlang_DQRNG(10, 3, 0.5)
*/
```

## License

GPL 3
