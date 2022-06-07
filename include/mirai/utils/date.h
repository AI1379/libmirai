#ifndef MIRAI_INCLUDE_MIRAI_UTILS_DATE_H_
#define MIRAI_INCLUDE_MIRAI_UTILS_DATE_H_

#include <chrono>

namespace mirai::utils::date {
inline std::uint32_t now() {
  auto now_time = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(now_time.time_since_epoch()).count();
}
}

#endif //MIRAI_INCLUDE_MIRAI_UTILS_DATE_H_
