/*
 * This file uses the Catch unit testing library, alongside
 * testthat's simple bindings, to test a C++ function.
 */

#include <random>
#include <testthat.h>

#include <RcppRNG/distribution/exponential_distribution.hpp>

template <typename _RealType>
using exponential_distribution =
    RcppRNG::distribution::exponential_distribution<_RealType>;

template <typename _RealType>
using param_t = typename exponential_distribution<_RealType>::param_type;

template <typename _RealType>
using std_param_t =
    typename std::exponential_distribution<_RealType>::param_type;

context("exponential_distributions can be initialized and re-parametrized") {
  auto lambda{0.5};

  test_that("is initialized from double with correct lambda-value") {
    exponential_distribution<double> expd{lambda};
    expect_true(expd.lambda() == lambda);
  }

  test_that("is initialized from param_type with correct lambda-value") {
    auto params = param_t<double>{lambda};
    exponential_distribution<double> expd{params};
    expect_true(expd.lambda() == lambda);
  }

  test_that(
      "is initialized from param_type - like object with correct "
      "lambda-value") {
    auto std_params = std_param_t<double>{lambda};
    auto params = param_t<double>{std_params};
    exponential_distribution<double> expd{params};
    expect_true(expd.lambda() == lambda);
  }

  test_that("lambda can be changed by providing new parameters") {
    exponential_distribution<double> expd{lambda};
    auto new_lambda{0.7};
    auto params = param_t<double>{new_lambda};
    expd.params(params);
    expect_true(expd.lambda() == new_lambda);
  }

}

context("exponential distributions can be compared") {
  auto lambda{0.5};
  exponential_distribution<double> expd{lambda};
  exponential_distribution<double> expd2{lambda};
  exponential_distribution<double> expd3{lambda*0.3};

  expect_true(expd == expd2);
  expect_true(expd != expd3);
}
