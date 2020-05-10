context("rexp")

use_seed <- 1623L
dqrng_flag <- FALSE

test_that("rexp with RcppRNG is equivalent to stats::rexp", {
  args <- list("rate" = 0.5)
  n <- 1e5
  expect_equal_sampling_result(Rcpp_rexp_RCPPRNG, stats::rexp, arguments = args,
    n=n, use_seed=use_seed)

  Rcpp_rexp_RCPPRNG_ <- function(n, rate) { # nolint
    sapply(1:n, function(x) Rcpp_rexp_RCPPRNG(1, rate))
  }
  rexp_ <- function(n, rate) {
    sapply(1:n, function(x) stats::rexp(1, rate))
  }

  expect_equal_sampling_result(Rcpp_rexp_RCPPRNG_, rexp_, arguments = args,
    n=n, use_seed=use_seed)

  x_ <- function(n, rate) {
    c(
      sapply(1:n, function(x) Rcpp_rexp_RCPPRNG(1, rate)),
      sapply(1:n, function(x) rexp(1, rate))
    )
  }
  y_ <- function(n, rate) {
    c(
      sapply(1:n, function(x) rexp(1, rate)),
      sapply(1:n, function(x) Rcpp_rexp_RCPPRNG(1, rate))
    )
  }
  expect_equal_sampling_result(x_, y_, arguments = args, n=n,
    use_seed=use_seed)
})


test_that("rexp with DQRNG is equivalent to dqrng::rexp", {
  skip_if_not_installed("dqrng")
  dqRNGkind("default")
  dqrng::dqRNGkind("default")
  args <- list("rate" = 0.5)
  n <- 1e5
  expect_equal_sampling_result(Rcpp_rexp_DQRNG, dqrng::dqrexp,
    arguments = args, n=n, use_seed=use_seed, dqrng_flag=TRUE)

  Rcpp_rexp_DQRNG_ <- function(n, rate) { # nolint
    sapply(1:n, function(x) Rcpp_rexp_DQRNG(1L, rate))
  }
  dqrexp_ <- function(n, rate) {
    sapply(1:n, function(x) dqrng::dqrexp(1, rate))
  }
  expect_equal_sampling_result(Rcpp_rexp_DQRNG_, dqrexp_,
    arguments = args, n=n, use_seed=use_seed, dqrng_flag=TRUE)

  x_ <- function(n, rate) {
    c(
      sapply(1:n, function(x) Rcpp_rexp_DQRNG(1, rate)),
      sapply(1:n, function(x) dqrng::dqrexp(1, rate))
    )
  }
  y_ <- function(n, rate) {
    c(
      sapply(1:n, function(x) dqrng::dqrexp(1, rate)),
      sapply(1:n, function(x) Rcpp_rexp_DQRNG(1, rate))
    )
  }
  expect_equal_sampling_result(x_, y_, arguments = args, n=n,
    use_seed=use_seed, dqrng_flag=TRUE)
})
