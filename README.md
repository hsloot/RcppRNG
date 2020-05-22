
<!-- README.md is generated from README.Rmd. Please edit that file -->

# RcppRNG

<!-- badges: start -->

[![R build
status](https://github.com/hsloot/RcppRNG/workflows/R-CMD-check/badge.svg)](https://github.com/hsloot/RcppRNG/actions)
[![Codecov test
coverage](https://codecov.io/gh/hsloot/RcppRNG/branch/master/graph/badge.svg)](https://codecov.io/gh/hsloot/RcppRNG?branch=master)
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
`Rcpp::RNGScope` with `RcppRNG::RcppRNG` which contains it’s own static
counter instead going through R’s Call API. Both version seem to be very
similar in their performance.

``` cpp
#include <Rcpp.h>

// [[Rcpp::plugins("cpp11")]]
// [[Rcpp::depends(RcppRNG,dqrng,BH,sitmo)]]

#include <RcppRNG.hpp>

using namespace Rcpp;

// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rexp_RNGScope(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  RcppRNG::ExpDistribution param(rate);
  RcppRNG::ExpGenerator<RNGScope> gen(param);
  std::generate(out.begin(), out.end(), gen);

  return out;
}

// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rexp_RcppRNG(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  RcppRNG::ExpDistribution param(rate);
  RcppRNG::ExpGenerator<RcppRNG::RcppRNG> gen(param);
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
#> 1 rexp(1e+05, 0.5)                 5.65ms   6.57ms      149.  783.79KB     2.07
#> 2 Rcpp_rexp_RNGScope(1e+05, 0.5)   3.85ms   4.58ms      214.    4.52MB     2.06
#> 3 Rcpp_rexp_RcppRNG(1e+05, 0.5)    3.85ms   4.82ms      195.  787.92KB     4.29
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
#> 1 rexp(1e+05, 0.5)                 5.47ms   6.08ms      157.     784KB     2.07
#> 2 Rcpp_rexp_RNGScope(1e+05, 0.5)   3.85ms   4.61ms      205.     786KB     2.05
#> 3 Rcpp_rexp_RcppRNG(1e+05, 0.5)    3.83ms   4.32ms      227.     784KB     4.24
#> 4 Rcpp_rexp_slow(1e+05, 0.5)       8.12ms    8.9ms      105.     784KB    45.7
```

However, we can also use another random number generator (e.g. the one
from `dqrng`):

``` cpp
#include <Rcpp.h>

// [[Rcpp::plugins("cpp11")]]
// [[Rcpp::depends(RcppRNG,dqrng,BH,sitmo)]]

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
  RcppRNG::ExpDistribution param(rate);
  RcppRNG::ExpGenerator<RcppRNG::DQRNG> gen(param);
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
#> 1 rexp(1e+05, 0.5)                 5.42ms   5.95ms      163.     784KB     2.06
#> 2 Rcpp_rexp_RNGScope(1e+05, 0.5)   3.83ms   4.63ms      208.     786KB     4.29
#> 3 Rcpp_rexp_RcppRNG(1e+05, 0.5)    3.89ms   4.34ms      224.     784KB     4.26
#> 4 Rcpp_rexp_DQRNG(1e+05, 0.5)    872.37µs   1.21ms      825.     781KB    14.1 
#> 5 Rcpp_rexp_slow(1e+05, 0.5)       8.13ms   8.76ms      109.     784KB    46.4
```

The main benefit of this design is that it allows us to implement new
sampling algorithms, which are based on basic generators (e.g. exp,
unif, …) such that they can be used with the base R RNG or other RNG’s
(e.g. dqrng’s). An example is described in a [create new
generators](articles/create-new-generators.html).

## License

GPL 3
