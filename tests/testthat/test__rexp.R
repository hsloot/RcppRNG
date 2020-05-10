context("rexp")

setup({
  # make sure that default RNGkind is used
  if (require_R_version("3.6.0")){
    RNGkind(kind="default", normal.kind="default", sample.kind="default")
  } else {
    RNGkind(kind="default", normal.kind="default")
  }

  if (requireNamespace("dqrng", quietly=TRUE)) {
    dqRNGkind("default")
    dqrng::dqRNGkind("default")
  }
})

use_seed <- 1623L

# sampling parameters
n <- 1e4
args <- list("rate" = 0.5)

test_that("rexp with Rcpp::RNGScope is equivalent to stats::rexp", {
  expect_equal_sampling_result(Rcpp_rexp_RNGScope, stats::rexp, arguments = args,
    n=n, use_seed=use_seed)

  Rcpp_rexp_RNGScope_ <- function(n, rate) { # nolint
    sapply(1:n, function(x) Rcpp_rexp_RNGScope(1, rate))
  }
  rexp_ <- function(n, rate) {
    sapply(1:n, function(x) stats::rexp(1, rate))
  }

  expect_equal_sampling_result(Rcpp_rexp_RNGScope_, rexp_, arguments = args,
    n=n, use_seed=use_seed)

  x_ <- function(n, rate) {
    c(
      sapply(1:n, function(x) Rcpp_rexp_RNGScope(1, rate)),
      sapply(1:n, function(x) rexp(1, rate))
    )
  }
  y_ <- function(n, rate) {
    c(
      sapply(1:n, function(x) rexp(1, rate)),
      sapply(1:n, function(x) Rcpp_rexp_RNGScope(1, rate))
    )
  }
  expect_equal_sampling_result(x_, y_, arguments = args, n=n,
    use_seed=use_seed)
})


test_that("rexp with RcppRNG is equivalent to stats::rexp", {
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
