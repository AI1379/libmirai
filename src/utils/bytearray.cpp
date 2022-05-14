#include "mirai/utils/bytearray.h"

#include "mirai/defs/macros.h"
#include "mirai/utils/encode.h"

#include <sstream>

namespace mirai {
using utils::ByteStream;
using utils::ByteArray;
// overload operator <<
ByteStream &operator<<(ByteStream &bs, const std::string &a) {
  for (std::string::value_type c : a) {
    bs.write(reinterpret_cast<mirai::byte *>(&c), 1);
  }
  return bs;
}
ByteStream &operator<<(ByteStream &bs, const char *c) {
  while ((*c) != '\0') {
    bs.write(reinterpret_cast<const mirai::byte *>(c++), 1);
  }
  return bs;
}
ByteStream &operator<<(ByteStream &bs, const byte &a) {
  bs.put(a);
  return bs;
}
ByteStream &operator<<(ByteStream &bs, const uint16_t &a) {
#if BYTEORDER_ENDIAN == BYTEORDER_LITTLE_ENDIAN
  auto x = reverseU16(a);
#elif BYTEORDER_ENDIAN == BYTEORDER_BIG_ENDIAN
  auto x = a;
#endif
  bs.write(reinterpret_cast<const mirai::byte *>(&x), 2);
  return bs;
}
ByteStream &operator<<(ByteStream &bs, const uint32_t &a) {
#if BYTEORDER_ENDIAN == BYTEORDER_LITTLE_ENDIAN
  auto x = reverseU32(a);
#elif BYTEORDER_ENDIAN == BYTEORDER_BIG_ENDIAN
  auto x = a;
#endif
  bs.write(reinterpret_cast<const mirai::byte *>(&x), 4);
  return bs;
}
ByteStream &operator<<(ByteStream &bs, const uint64_t &a) {
#if BYTEORDER_ENDIAN == BYTEORDER_LITTLE_ENDIAN
  auto x = reverseU64(a);
#elif BYTEORDER_ENDIAN == BYTEORDER_BIG_ENDIAN
  auto x = a;
#endif
  bs.write(reinterpret_cast<const mirai::byte *>(&x), 8);
  return bs;
}
ByteStream &operator<<(ByteStream &bs, const int8_t &a) {
  return bs << static_cast<const uint8_t>(a);
}
ByteStream &operator<<(ByteStream &bs, const int16_t &a) {
  return bs << static_cast<const uint16_t>(a);
}
ByteStream &operator<<(ByteStream &bs, const int32_t &a) {
  return bs << static_cast<const uint32_t>(a);
}
ByteStream &operator<<(ByteStream &bs, const int64_t &a) {
  return bs << static_cast<const uint64_t>(a);
}
ByteStream &operator<<(ByteStream &bs, const ByteArray &buf) {
  bs.write(buf.data(), buf.size());
  return bs;
}

// overload operator >>
ByteStream &operator>>(ByteStream &bs, byte &a) {
  a = bs.get();
  return bs;
}
ByteStream &operator>>(ByteStream &bs, uint16_t &a) {
  a = (static_cast<uint16_t>(bs.get()) << 4) |
      static_cast<uint16_t>(bs.get());
  return bs;
}
ByteStream &operator>>(ByteStream &bs, uint32_t &a) {
  a = (static_cast<uint32_t>(bs.get()) << 24) |
      (static_cast<uint32_t>(bs.get()) << 16) |
      (static_cast<uint32_t>(bs.get()) << 8) |
      (static_cast<uint32_t>(bs.get()));
  return bs;
}
ByteStream &operator>>(ByteStream &bs, uint64_t &a) {
  a = (static_cast<uint64_t>(bs.get()) << 56) |
      (static_cast<uint64_t>(bs.get()) << 48) |
      (static_cast<uint64_t>(bs.get()) << 40) |
      (static_cast<uint64_t>(bs.get()) << 32) |
      (static_cast<uint64_t>(bs.get()) << 24) |
      (static_cast<uint64_t>(bs.get()) << 16) |
      (static_cast<uint64_t>(bs.get()) << 8) |
      (static_cast<uint64_t>(bs.get()));
  return bs;
}
ByteStream &operator>>(ByteStream &bs, int16_t &a) {
  return bs >> (uint16_t &) (a);
};
ByteStream &operator>>(ByteStream &bs, int32_t &a) {
  return bs >> (uint32_t &) (a);
}
ByteStream &operator>>(ByteStream &bs, int64_t &a) {
  return bs >> (uint64_t &) (a);
}

}

std::size_t ByteArrayHashFunc(const mirai::utils::ByteArray &buf){
  // TODO: rewrite ByteArrayHashFunc
  return std::hash<std::string>{}(mirai::utils::base64Encode(buf));
}