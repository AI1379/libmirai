//
// Created by Arendellian13 on 2022/4/30.
//

#ifndef MIRAI_INCLUDE_MIRAI_UTILS_BYTEARRAY_H_
#define MIRAI_INCLUDE_MIRAI_UTILS_BYTEARRAY_H_

#include "mirai/defs/types.h"

#include <algorithm>
#include <string>

namespace mirai::utils {
// TODO: 写一个自有的ByteArray类
using ByteArray = std::basic_string<mirai::byte>;
using ByteArrayStream = std::basic_stringstream<mirai::byte>;
using ByteArrayView = std::basic_string_view<mirai::byte>;

inline ByteArray toByteArray(const std::string &str) {
  return ByteArray((byte *) (str.data()), str.size());
}

inline uint32_t readUInt32BE(const ByteArray &buf, std::size_t idx) {
  uint32_t res = 0;
  for (size_t i = 0; i < 4; i++) {
    res = (res << 8) | buf[i + idx];
  }
  return res;
}

inline void writeUInt32BE(ByteArray &buf, std::size_t idx, uint32_t val) {
  for (int i = 0; i < 4; i++) {
    buf[idx + i] = (val >> 24);
    val <<= 8;
  }
}

}

#endif //MIRAI_INCLUDE_MIRAI_UTILS_BYTEARRAY_H_
