#include <functional>
#include <iostream>

int main() {
#ifndef __cpp_lib_bind_back
  static_assert(false, "`__cpp_lib_bind_back` is not defined");
#endif
  std::cout << "OK" << std::endl;
  return 0;
}
