#ifndef MIRAI_INCLUDE_MIRAI_UTILS_STRING_FORMAT_H_
#define MIRAI_INCLUDE_MIRAI_UTILS_STRING_FORMAT_H_

#include "mirai/utils/bytearray.h"

#include <string>
#include <memory>

namespace mirai::utils {

// Original Source: https://gist.github.com/Zitrax/a2e0040d301bf4b8ef8101c0b1e3f1d5
namespace details {
template<typename T>
auto convert(T &&t) {
  if constexpr (std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value) {
    return std::forward<T>(t).c_str();
  } else if constexpr(std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, utils::ByteArray>) {
    return reinterpret_cast<const char*>(std::forward<T>(t).data());
  } else {
    return std::forward<T>(t);
  }
}

template<typename ... Args>
std::string stringFormatInternal(const std::string &format, Args &&... args) {
  const auto size = snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args) ...) + 1;
  if (size <= 0) { throw std::runtime_error("Error during formatting."); }
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format.c_str(), args ...);
  return std::string(buf.get(), buf.get() + size - 1);
}
}

template<typename ... Args>
std::string stringFormat(std::string fmt, Args &&... args) {
  return details::stringFormatInternal(fmt, details::convert(std::forward<Args>(args))...);
}

}

#endif //MIRAI_INCLUDE_MIRAI_UTILS_STRING_FORMAT_H_
