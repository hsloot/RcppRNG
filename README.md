
<!-- README.md is generated from README.Rmd. Please edit that file -->

# RcppRNG

<!-- badges: start -->

<!-- badges: end -->

The goal of RcppRNG is to explore a templated implementation of Rcpp’s
internal generators for sampling. The motivation is to be able to write
sampling routines that are based on native sampling routines (`rexp`,
`runif`, …) in a way that one can easily switch to an alternative RNG
(e.g. `dqrng`).

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

// [[Rcpp::depends(RcppRNG)]]
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
#> 1 rexp(1e+05, 0.5)                 5.05ms   5.13ms      195.        NA     2.41
#> 2 Rcpp_rexp_RNGScope(1e+05, 0.5)   3.34ms   3.91ms      240.        NA     2.07
#> 3 Rcpp_rexp_RcppRNG(1e+05, 0.5)    3.37ms   3.95ms      255.        NA     4.17
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
#> 1 rexp(1e+05, 0.5)                 4.71ms   5.06ms      111.        NA     0   
#> 2 Rcpp_rexp_RNGScope(1e+05, 0.5)   3.33ms   3.88ms      269.        NA     4.20
#> 3 Rcpp_rexp_RcppRNG(1e+05, 0.5)    3.37ms   3.96ms      264.        NA     4.16
#> 4 Rcpp_rexp_slow(1e+05, 0.5)       8.54ms   8.82ms      107.        NA    72.9
```

## License

GPL 3
