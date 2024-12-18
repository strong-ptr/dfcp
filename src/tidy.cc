#include <stdio.h>
#include <tidy.h>
#include <tidybuffio.h>

#include "tidy.hh"

// See: https://www.html-tidy.org/developer/
std::optional<std::string> html_to_xhtml(const std::string &html) {
  /*
   * From the docs:
   *
   * Before we look at the code, it’s important to understand that API
   * functions that return an integer almost universally adhere to the
   * following convention:
   *
   * - 0 == Success
   *   Good to go.
   * - 1 == Warnings, but no errors
   *   Check the error buffer or track error messages for details.
   * - 2 == Errors (and maybe warnings)
   *   By default, Tidy will not produce output. You can force output with the
   *   `TidyForceOutput` option. As with warnings, check the error buffer or
   *   track error messages for details.
   * - <0 == Severe error
   *   Usually value equals -errno. See errno.h.
   *
   * Also, by default, warning and error messages are sent to stderr. You can
   * redirect diagnostic output using either `tidySetErrorFile()` or
   * `tidySetErrorBuffer()`. See `tidy.h` for details.
   */
  const char *cstr{html.c_str()};
  TidyBuffer output{};
  TidyBuffer errbuf{};
  int rc = -1;
  Bool ok;

  // Initialize "document"
  TidyDoc tdoc = tidyCreate();
  printf("Tidying:\t%s\n", cstr);

  // Convert to XHTML
  ok = tidyOptSetBool(tdoc, TidyXhtmlOut, yes);
  if (ok) {
    // Capture diagnostics
    rc = tidySetErrorBuffer(tdoc, &errbuf);
  }
  if (rc >= 0) {
    // Parse the input
    rc = tidyParseString(tdoc, cstr);
  }
  if (rc >= 0) {
    // Tidy it up!
    rc = tidyCleanAndRepair(tdoc);
  }
  if (rc >= 0) {
    // Kvetch
    rc = tidyRunDiagnostics(tdoc);
  }
  if (rc > 1) {
    // If error, force output.
    rc = (tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1);
  }
  if (rc >= 0) {
    // Pretty Print
    rc = tidySaveBuffer(tdoc, &output);
  }

  if (rc == 1) {
    fprintf(stderr, "Warning occured: Diagnostics:\n%s", errbuf.bp);
  } else if (rc == 2) {
    fprintf(stderr, "Error occured: Diagnostics:\n%s", errbuf.bp);
  } else if (rc < 0) {
    fprintf(stderr, "A severe error (%d) occurred.\n", rc);
    fprintf(stderr, "errno: %s\n", strerror(errno));
  }
  std::string res{reinterpret_cast<char *>(output.bp)};
  tidyBufFree(&output);
  tidyBufFree(&errbuf);
  tidyRelease(tdoc);

  if (rc == 0 || rc == 1) {
    return res;
  }
  return {};
}
