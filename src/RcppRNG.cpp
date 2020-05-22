#include <Rcpp.h>
#include <RcppRNG.hpp>
using namespace Rcpp;

namespace {
RcppRNG::DQRNG shared_dqrng = RcppRNG::DQRNG();
}

//' Set the seed for the DQRNG
//'
//' Original in \url{https://github.com/daqana/dqrng/blob/master/src/dqrng.cpp}
//'
//' @inheritParams dqrng::dqset.seed
//'
//' @seealso \code{\link[dqrng]{dqset.seed}}
//' @export
// [[Rcpp::export(rng=false)]]
void dqset_seed(Rcpp::IntegerVector seed, Rcpp::Nullable<Rcpp::IntegerVector> stream = R_NilValue) {
  uint64_t _seed = dqrng::convert_seed<uint64_t>(seed);
  if (stream.isNotNull()) {
    uint64_t _stream = dqrng::convert_seed<uint64_t>(stream.as());
    shared_dqrng.shared_rng->seed(_seed, _stream);
  } else {
    shared_dqrng.shared_rng->seed(_seed);
  }
}

//' Choose RNGkind for DQRNG
//'
//' Original in \url{https://github.com/daqana/dqrng/blob/master/src/dqrng.cpp}
//'
//' @inheritParams dqrng::dqRNGkind
//'
//' @seealso \code{\link[dqrng]{dqRNGkind}}
//'
//' @export
// [[Rcpp::export(rng = false)]]
void dqRNGkind(std::string kind, const std::string& normal_kind = "ignored") {
  for (auto & c: kind)
    c = std::tolower(c);
  uint64_t seed = shared_dqrng.shared_rng->operator()();
  if (kind == "default") {
    shared_dqrng.shared_rng =  dqrng::generator(seed);
  } else if (kind == "xoroshiro128+") {
    shared_dqrng.shared_rng =  dqrng::generator<dqrng::xoroshiro128plus>(seed);
  } else if (kind == "xoshiro256+") {
    shared_dqrng.shared_rng =  dqrng::generator<dqrng::xoshiro256plus>(seed);
  } else if (kind == "pcg64") {
    shared_dqrng.shared_rng =  dqrng::generator<pcg64>(seed);
  } else if (kind == "threefry") {
    shared_dqrng.shared_rng =  dqrng::generator<sitmo::threefry_20_64>(seed);
  } else {
    Rcpp::stop("Unknown random generator kind: %s", kind);
  }
}

//' Alternatives for \code{rexp}
//'
//' @param n Number of samples
//' @param rate Rate parameter
//'
//' @name rexp-alternative
//' @export
// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rexp_RNGScope(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  RcppRNG::ExpDistribution<double> param(rate);
  RcppRNG::ExpGenerator<Rcpp::RNGScope> gen(param);
  std::generate(out.begin(), out.end(), gen);

  return out;
}

//' @rdname rexp-alternative
//' @export
// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rexp_RCPPRNG(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  RcppRNG::ExpDistribution<double> param(rate);
  RcppRNG::ExpGenerator<RcppRNG::RcppRNG> gen(param);
  std::generate(out.begin(), out.end(), gen);

  return out;
}

//' @rdname rexp-alternative
//' @export
// [[Rcpp::export(rng=false)]]
NumericVector Rcpp_rexp_DQRNG(R_xlen_t n, double rate = 1.) {
  NumericVector out(no_init(n));
  RcppRNG::ExpDistribution<double> param(rate);
  RcppRNG::ExpGenerator<RcppRNG::DQRNG> gen(param);
  std::generate(out.begin(), out.end(), gen);

  return out;
}
