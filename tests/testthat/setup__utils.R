require_R_version <- function(version_string = paste0(major, minor, sep="."), # nolint
    major = R.version$major, minor = R.version$minor) {
  R_version_string <- paste(R.version$major, R.version$minor, sep=".") # nolint
  1 == compareVersion(R_version_string, version_string)
}
