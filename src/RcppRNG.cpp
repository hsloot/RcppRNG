#include <Rcpp.h>
#include <RcppRNG.hpp>
using namespace Rcpp;

//' \code{rexp} implementations in Rcpp for benchmarking
//'
//' These functions serve no real purpose except demonstrating
//' how to use the RNG Generators and providing some functions
//' for benchmarks.
//'
//' @inheritParams stats::rexp
//'
//' @name rexp-Rcpp


//' @rdname rexp-Rcpp
//'
//' @export
// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rexp1(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  double scale = 1. / rate;
  RcppRNG::RcppRNG rng;
  for (R_xlen_t k=0; k<n; k++)
    out[k] = R::exp_rand() * scale;

  return out;
}

//' @rdname rexp-Rcpp
//'
//' @export
// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rexp2(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  RcppRNG::ExpGenerator<RcppRNG::RcppRNG> gen(rate);
  for (R_xlen_t k=0; k<n; k++)
    out[k] = gen();

  return out;
}

//' @rdname rexp-Rcpp
//'
//' @export
// [[Rcpp::export(rng=true)]]
NumericVector Rcpp_rexp3(R_xlen_t n, double rate = 1.) {
  return rexp(n, rate);
}

//' @rdname rexp-Rcpp
//'
//' @export
// [[Rcpp::export(rng=true)]]
NumericVector Rcpp_rexp4(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  for (R_xlen_t k=0; k<n; k++)
    out[k] = rexp(1, rate)[0];
  return out;
}

//' @rdname rexp-Rcpp
//'
//' @export
// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rexp5(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  RcppRNG::ExpGenerator<Rcpp::RNGScope> gen(rate);
  for (R_xlen_t k=0; k<n; k++)
    out[k] = gen();

  return out;
}
