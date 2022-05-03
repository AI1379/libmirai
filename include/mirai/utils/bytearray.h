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
using ByteStream = std::basic_stringstream<mirai::byte>;
using ByteArrayView = std::basic_string_view<mirai::byte>;

inline ByteArray toByteArray(const std::string &str) {
  return ByteArray((byte *) (str.data()), str.size());
}

inline std::string toString(const ByteArray &buf) {
  return std::string((char *) (buf.data()), buf.size());
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

namespace mirai{
// overload operator <<
utils::ByteStream &operator<<(utils::ByteStream &bs, const std::string &a);
utils::ByteStream &operator<<(utils::ByteStream &bs, const char *c);
utils::ByteStream &operator<<(utils::ByteStream &bs, const byte &a);
// Default: Big Endian
utils::ByteStream &operator<<(utils::ByteStream &bs, const uint16_t &a);
utils::ByteStream &operator<<(utils::ByteStream &bs, const uint32_t &a);
utils::ByteStream &operator<<(utils::ByteStream &bs, const uint64_t &a);
utils::ByteStream &operator<<(utils::ByteStream &bs, const int8_t &a);
utils::ByteStream &operator<<(utils::ByteStream &bs, const int16_t &a);
utils::ByteStream &operator<<(utils::ByteStream &bs, const int32_t &a);
utils::ByteStream &operator<<(utils::ByteStream &bs, const int64_t &a);
utils::ByteStream &operator<<(utils::ByteStream &bs, const utils::ByteArray &buf);

// overload operator >>
utils::ByteStream &operator>>(utils::ByteStream &bs, byte &a);
utils::ByteStream &operator>>(utils::ByteStream &bs, uint16_t &a);
utils::ByteStream &operator>>(utils::ByteStream &bs, uint32_t &a);
utils::ByteStream &operator>>(utils::ByteStream &bs, uint64_t &a);
utils::ByteStream &operator>>(utils::ByteStream &bs, int16_t &a);
utils::ByteStream &operator>>(utils::ByteStream &bs, int32_t &a);
utils::ByteStream &operator>>(utils::ByteStream &bs, int64_t &a);
}

#endif //MIRAI_INCLUDE_MIRAI_UTILS_BYTEARRAY_H_
