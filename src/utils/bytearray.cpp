#include "mirai/utils/bytearray.h"

#include <sstream>

namespace mirai::utils {
class ByteStream : public std::basic_stringstream<mirai::byte> {
 public:
  ByteStream &operator<<(const std::string &a) {
    for (std::string::value_type c : a) {
      this->write(reinterpret_cast<mirai::byte *>(&c), 1);
    }
    return *this;
  }
  ByteStream &operator<<(const char *c) {
    while ((*c) != '\0') {
      this->write(reinterpret_cast<const mirai::byte *>(c++), 1);
    }
    return *this;
  }
  ByteStream &operator<<(const uint16_t &a) {
    auto x = reverseU16(a);
    this->write(reinterpret_cast<const mirai::byte *>(&x), 4);
    return *this;
  }
  ByteStream &operator<<(const uint32_t &a) {
    auto x = reverseU32(a);
    this->write(reinterpret_cast<const mirai::byte *>(&x), 4);
    return *this;
  }
  ByteStream &operator<<(const uint64_t &a) {
    auto x = reverseU64(a);
    this->write(reinterpret_cast<const mirai::byte *>(&x), 8);
    return *this;
  }
  ByteStream &operator<<(const int16_t &a) {
    return (*this) << static_cast<const uint16_t>(a);
  }
  ByteStream &operator<<(const int32_t &a) {
    return (*this) << static_cast<const uint32_t>(a);
  }
  ByteStream &operator<<(const int64_t &a) {
    return (*this) << static_cast<const uint64_t>(a);
  }
  ByteStream &operator<<(const ByteArray &buf) {
    this->write(buf.data(), buf.size());
    return *this;
  }
};
}