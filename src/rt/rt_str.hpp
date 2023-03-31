#ifndef RT_STR_HPP
#define RT_STR_HPP

#include <stdio.h>
#include <string>

namespace rt {
namespace str {

// String formatting utility function that abstracts away
// needing to use snprintf.
// Reference https://stackoverflow.com/a/26221725/5071723
template <typename... Args>
std::string format(const std::string &format,
                   Args... args) {
  // Extra space for '\0' so plus 1
  int size =
      std::snprintf(nullptr, 0, format.c_str(), args...) +
      1;
  char *buf = new char[static_cast<size_t>(size)]();
  std::snprintf(buf, size, format.c_str(), args...);
  // We don't want the '\0' inside so subtract 1
  return {buf, buf + size - 1};
}

// Proxy format function for char* inputs
// Reference https://stackoverflow.com/a/26221725/5071723
template <typename... Args>
std::string format(const char *format, Args... args) {
  return str::format(std::string(format), args...);
}

}  // namespace str
}  // namespace rt

#endif  // RT_STR_HPP