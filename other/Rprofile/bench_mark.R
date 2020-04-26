library(RcppRNG)
library(bench)

n <- 1e5
rate <- 0.5
bench::mark(
  Rcpp1 = Rcpp_rexp1(n, rate),
  Rcpp2 = Rcpp_rexp2(n, rate),
  Rcpp3 = Rcpp_rexp4(n, rate),
  Rcpp4 = Rcpp_rexp4(n, rate),
  Rcpp5 = Rcpp_rexp5(n, rate),
  R = rexp(n, rate),
  check=FALSE
)
