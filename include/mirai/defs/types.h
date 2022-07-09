//
// Created by Arendellian13 on 2022/4/30.
//

#ifndef MIRAI_INCLUDE_MIRAI_DEFS_TYPES_H_
#define MIRAI_INCLUDE_MIRAI_DEFS_TYPES_H_

#include <cstdint>
#include <cstring>
#include <type_traits>
#include <iostream>

namespace mirai {

using byte = std::uint8_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

inline mirai::byte operator ""_byte(std::uint64_t val) { return static_cast<mirai::byte>(val); }
inline std::uint16_t operator ""_u16(std::uint64_t val) { return static_cast<std::uint16_t>(val); }
inline std::uint32_t operator ""_u32(std::uint64_t val) { return static_cast<std::uint32_t>(val); }
inline std::uint64_t operator ""_u64(std::uint64_t val) { return static_cast<std::uint64_t>(val); }
inline std::int16_t operator ""_s16(std::uint64_t val) { return static_cast<std::int16_t>(val); }
inline std::int32_t operator ""_s32(std::uint64_t val) { return static_cast<std::int32_t>(val); }
inline std::int64_t operator ""_s64(std::uint64_t val) { return static_cast<std::int64_t>(val); }

inline uint16_t reverseU16(uint16_t x) {
  return (x >> 8) | (x << 8);
}
inline uint32_t reverseU32(uint32_t x) {
  return ((x >> 24) & 0xff) |
      ((x << 8) & 0xff0000) |
      ((x >> 8) & 0xff00) |
      ((x << 24) & 0xff000000);
}
inline uint64_t reverseU64(uint64_t x) {
  x = ((x << 8) & 0xFF00FF00FF00FF00ULL) | ((x >> 8) & 0x00FF00FF00FF00FFULL);
  x = ((x << 16) & 0xFFFF0000FFFF0000ULL) | ((x >> 16) & 0x0000FFFF0000FFFFULL);
  return (x << 32) | (x >> 32);
}

}

// Extend std
namespace std {
template<>
struct char_traits<mirai::byte> {
  using char_type = mirai::byte;
  using int_type = std::uint8_t;
  using pos_type = streampos;
  using off_type = streamoff;
  using state_type = mbstate_t;
  static constexpr void assign(char_type &c1, const char_type &c2) noexcept { c1 = c2; }
  static constexpr bool eq(const char_type &c1, const char_type &c2) noexcept { return c1 == c2; }
  static constexpr bool lt(const char_type &c1, const char_type &c2) noexcept { return c1 < c2; }
  static constexpr int compare(const char_type *s1, const char_type *s2, size_t n) {
    while (n--) {
      if ((*s1) < (*s2)) return -1;
      if ((*s1) > (*s2)) return 1;
      s1++;
      s2++;
    }
    return 0;
  }
  static constexpr int_type eof() noexcept { return static_cast<int_type >(-1); }
  static constexpr int_type not_eof(const int_type &c) noexcept { return (c == eof()) ? 0 : c; }
  static constexpr size_t length(const char_type *s) {
    size_t res = 0;
    while ((*s) != eof()) {
      s++;
      res++;
    }
    return res;
  }
  static constexpr const char_type *find(const char_type *s, size_t n, const char_type &c) {
    if (n == 0) return nullptr;
    while ((*s) != eof() && !eq((*s), c))s++;
    return s;
  }
  static constexpr char_type *move(char_type *s1, const char_type *s2, size_t n) {
    if (n == 0) return s1;
    return static_cast<char_type *>(memmove(s1, s2, n));
  }
  static constexpr char_type *copy(char_type *s1, const char_type *s2, size_t n) {
    if (n == 0)return s1;
    return static_cast<char_type *>(memcpy(s1, s2, n));
  }
  static constexpr char_type to_char_type(const int_type &c) noexcept { return static_cast<char_type>(c); }
  static constexpr int_type to_int_type(const char_type &c) noexcept { return static_cast<int_type>(c); }
  static constexpr bool eq_int_type(const int_type &c1, const int_type &c2) noexcept { return c1 == c2; }
  static constexpr char_type *assign(char_type *s, size_t n, char_type a) {
    if (n == 0)return s;
    return static_cast<char_type *>(memset(s, a, n));
  }
};
}

#endif //MIRAI_INCLUDE_MIRAI_DEFS_TYPES_H_
