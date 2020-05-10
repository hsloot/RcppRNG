format_args <- function(args, ...) {
  paste(paste0(" * ", names(args), " = \n\t", format(args, ...)), collapse = "\n")
}

expect_equal_sampling_result <- function(object, expected, arguments, n,
  use_seed = 1623L, dqrng_flag = FALSE, ...) {
  act <- testthat::quasi_label(rlang::enquo(object), NULL, arg = "object")
  exp <- testthat::quasi_label(rlang::enquo(expected), NULL, arg = "expected")
  args <- testthat::quasi_label(rlang::enquo(arguments), "Arguments",
    arg = "arguments")

  assertthat::assert_that(rlang::is_closure(act$val), rlang::is_closure(exp$val),
    missing(n) || assertthat::is.count(n), assertthat::is.count(use_seed))

  if (!missing(n)) {
    arg_list <- c("n" = n, args$val)
  } else {
    arg_list <- args$val
  }
  if (!dqrng_flag)
    set.seed(use_seed)
  else {
    RcppRNG::dqset_seed(use_seed)
    dqrng::dqset.seed(use_seed)
  }
  x <- do.call(act$val, args = arg_list)
  if (!dqrng_flag)
    set.seed(use_seed)
  else {
    stopifnot(requireNamespace("dqrng", quietly = TRUE))
    RcppRNG::dqset_seed(use_seed)
    dqrng::dqset.seed(use_seed)
  }
  y <- do.call(exp$val, args = arg_list)

  comp <- testthat::compare(x, y, ...)
  testthat::expect(
    comp$equal,
    sprintf(paste0(
      "Sample results of %s not equal to those of %s.",
      "\nSeed: %s",
      "\nNumber of simulations: %s",
      "\n%s:",
      "\n%s",
      "\n\n%s"
    ), act$lab, exp$lab, use_seed, ifelse(!missing(n), n, 1L), args$lab,
    format_args(args$val, justify = "right", digits = 2L), comp$message),
    info = NULL
  )

  invisible(act$val)
}
