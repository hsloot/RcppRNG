# adapted from https://github.com/daqana/dqrng/blob/53c5302/tests/testthat/test-generators.R
context("dqrng--clone generators")

seed <- 1234567890

test_that("Default generator: setting seed produces identical uniformly distributed numbers", {
  dqRNGkind("default")
  dqset_seed(seed)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_equal(u1, u2)
})

test_that("Default generator: setting seed and stream produces identical uniformly distributed numbers", {
  dqRNGkind("default")
  dqset_seed(seed, 1)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed, 1)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_equal(u1, u2)
})

test_that("Default generator: setting same seed but different stream produces different uniformly distributed numbers", {
  dqRNGkind("default")
  dqset_seed(seed, 1)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed, 2)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_false(identical(u1, u2))
})

test_that("PCG64: setting seed produces identical uniformly distributed numbers", {
  dqRNGkind("pcg64")
  dqset_seed(seed)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_equal(u1, u2)
})

test_that("PCG64: setting seed and stream produces identical uniformly distributed numbers", {
  dqRNGkind("pcg64")
  dqset_seed(seed, 1)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed, 1)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_equal(u1, u2)
})

test_that("PCG64: setting same seed but different stream produces different uniformly distributed numbers", {
  dqRNGkind("pcg64")
  dqset_seed(seed, 1)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed, 2)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_false(identical(u1, u2))
})

test_that("Xoroshiro128: setting seed produces identical uniformly distributed numbers", {
  dqRNGkind("Xoroshiro128+")
  dqset_seed(seed)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_equal(u1, u2)
})

test_that("Xoroshiro128: setting seed and stream produces identical uniformly distributed numbers", {
  dqRNGkind("Xoroshiro128+")
  dqset_seed(seed, 1)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed, 1)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_equal(u1, u2)
})

test_that("Xoroshiro128: setting same seed but different stream produces different uniformly distributed numbers", {
  dqRNGkind("Xoroshiro128+")
  dqset_seed(seed, 1)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed, 2)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_false(identical(u1, u2))
})

test_that("Xoshiro256: setting seed produces identical uniformly distributed numbers", {
  dqRNGkind("Xoshiro256+")
  dqset_seed(seed)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_equal(u1, u2)
})

test_that("Xoshiro256: setting seed and stream produces identical uniformly distributed numbers", {
  dqRNGkind("Xoshiro256+")
  dqset_seed(seed, 1)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed, 1)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_equal(u1, u2)
})

test_that("Xoshiro256: setting same seed but different stream produces different uniformly distributed numbers", {
  dqRNGkind("Xoshiro256+")
  dqset_seed(seed, 1)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed, 2)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_false(identical(u1, u2))
})

test_that("Threefry: setting seed produces identical uniformly distributed numbers", {
  dqRNGkind("Threefry")
  dqset_seed(seed)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_equal(u1, u2)
})

test_that("Threefry: setting seed and stream produces identical uniformly distributed numbers", {
  dqRNGkind("Threefry")
  dqset_seed(seed, 1)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed, 1)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_equal(u1, u2)
})

test_that("Threefry: setting same seed but different stream produces different uniformly distributed numbers", {
  dqRNGkind("Threefry")
  dqset_seed(seed, 1)
  u1 <- Rcpp_rexp_DQRNG(10)
  dqset_seed(seed, 2)
  u2 <- Rcpp_rexp_DQRNG(10)
  expect_false(identical(u1, u2))
})

test_that("non-existant RNG produces error", {
  expect_error(dqRNGkind("does_not_exist"))
})
