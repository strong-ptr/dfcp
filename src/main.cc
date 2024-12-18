#include "tidy.hh"
#include <iostream>
#include <string>

// Most important next steps:
// - [ ] Implement logger
// - [ ] Replace all prints with loggers

int main() {
  std::string input{"<title>Foo</title><p>Foo!"};
  auto res{html_to_xhtml(input)};
  if (res) {
    std::cout << *res << std::endl;
  }
  return 0;
}
