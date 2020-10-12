
<!-- README.md is generated from README.Rmd. Please edit that file -->

RcppRNG
=======

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

Installation
------------

Warning: This package is currently only indented as a showcase. If you
want to replicate the benchmarks in this package, you can install it via

    remotes::install_github("hsloot/RcppRNG")

Demonstration
-------------

At the moment, only a templated version of the generator for the
exponential function is implemented. The generator template allows
variations for difference RNG classes (which are essentially assumed to
behave like `Rcpp::RNGScope`). I implemented my own version of
`Rcpp::RNGScope` with `RcppRNG::RcppRNG` which contains it’s own static
counter instead going through R’s Call API. Both version seem to be very
similar in their performance.

    #include <Rcpp.h>

    // [[Rcpp::plugins("cpp14")]]
    // [[Rcpp::depends(RcppRNG,dqrng,BH,sitmo)]]

    #include <RcppRNG.hpp>

    using namespace Rcpp;

    using unit_exponential_distribution =
        RcppRNG::unit_exponential_distribution<double>;
    using exponential_distribution =
        RcppRNG::exponential_distribution<double, unit_exponential_distribution>;

    // [[Rcpp::export(rng=false)]]
    NumericVector Rcpp_rexp_RNGScope(R_xlen_t n, double rate = 1.) {
      NumericVector out(no_init(n));
      Rcpp::RNGScope engine{};
      exponential_distribution exp{rate};
      std::generate(out.begin(), out.end(),
                    [&engine, &exp]() { return exp(engine); });

      return out;
    }

    // [[Rcpp::export(rng=false)]]
    NumericVector Rcpp_rexp_RcppRNG(R_xlen_t n, double rate = 1.) {
      NumericVector out(no_init(n));
      RcppRNG::rng::RcppRNG engine{};
      exponential_distribution exp{rate};
      std::generate(out.begin(), out.end(),
                    [&engine, &exp]() { return exp(engine); });

      return out;
    }

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
    #> 1 rexp(1e+05, 0.5)                 6.87ms    9.2ms      107.  783.79KB     0   
    #> 2 Rcpp_rexp_RNGScope(1e+05, 0.5)   4.52ms   6.76ms      140.    4.52MB     2.09
    #> 3 Rcpp_rexp_RcppRNG(1e+05, 0.5)    4.58ms   6.77ms      144.  787.92KB     2.06

Why is that useful?
-------------------

For the exponential distribution, the problem is not that easy to
motivate, since whenever one wants repeated single exponential samples
inside the C++ function, one could resort to

    scale * R::exp_rand();

However, it is much more difficult, if you need repeated single samples
from a discrete distribution (e.g. to sample the transitions in a Markov
process). At his point you could do somethings like

    sample(n, 1, false, probs, true)[0]; // probs is NumericVector of size n

This, however, would construct a `NumericVector` object in each draw.
This has a significant overhead. For the exponential distribution this
will be demonstrated in the following

    #include <Rcpp.h>
    using namespace Rcpp;

    // [[Rcpp::export]]
    NumericVector Rcpp_rexp_slow(R_xlen_t n, double rate = 1.) {
      NumericVector out(no_init(n));
      for (R_xlen_t k=0; k<n; k++)
        out[k] = rexp(1, rate)[0];

      return out;
    }

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
    #> 1 rexp(1e+05, 0.5)                 6.76ms   9.35ms     103.      784KB     0   
    #> 2 Rcpp_rexp_RNGScope(1e+05, 0.5)   5.25ms   6.44ms     150.      784KB     2.08
    #> 3 Rcpp_rexp_RcppRNG(1e+05, 0.5)    5.45ms   7.52ms     123.      784KB     2.05
    #> 4 Rcpp_rexp_slow(1e+05, 0.5)      12.92ms  19.02ms      51.0     784KB    22.3

However, we can also use another random number generator (e.g. the one
from `dqrng`):

    #include <Rcpp.h>

    // [[Rcpp::plugins("cpp14")]]
    // [[Rcpp::depends(RcppRNG,dqrng,BH,sitmo)]]

    #include <RcppRNG.hpp>

    using namespace Rcpp;

    using unit_exponential_distribution =
        RcppRNG::unit_exponential_distribution<double>;
    using exponential_distribution =
        RcppRNG::exponential_distribution<double, unit_exponential_distribution>;

    RcppRNG::rng::DQRNG shared_dqrng = RcppRNG::rng::DQRNG();

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
      RcppRNG::rng::DQRNG engine{};
      exponential_distribution exp{rate};
      std::generate(out.begin(), out.end(),
                    [&engine, &exp]() { return exp(engine); });

      return out;
    }

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
    #> 1 rexp(1e+05, 0.5)                 8.57ms   9.59ms     102.      784KB     2.00
    #> 2 Rcpp_rexp_RNGScope(1e+05, 0.5)   5.79ms   6.98ms     142.      784KB     2.06
    #> 3 Rcpp_rexp_RcppRNG(1e+05, 0.5)    5.32ms   7.68ms     127.      784KB     2.09
    #> 4 Rcpp_rexp_DQRNG(1e+05, 0.5)      1.22ms   2.69ms     349.      781KB     4.39
    #> 5 Rcpp_rexp_slow(1e+05, 0.5)      14.94ms     19ms      53.4     784KB    22.0

The main benefit of this design is that it allows us to implement new
sampling algorithms, which are based on basic generators (e.g. exp,
unif, …) such that they can be used with the base R RNG or other RNG’s
(e.g. dqrng’s). An example is described in a [create new
generators](articles/create-new-generators.html).

License
-------

GPL 3
