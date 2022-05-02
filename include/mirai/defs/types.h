//
// Created by Arendellian13 on 2022/4/30.
//

#ifndef MIRAI_INCLUDE_MIRAI_DEFS_TYPES_H_
#define MIRAI_INCLUDE_MIRAI_DEFS_TYPES_H_

#include <cstdint>
#include <cstring>
#include <type_traits>

namespace mirai {

using byte = std::uint8_t;

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
  static constexpr int compare(const char_type *s1, const char_type *s2, ::size_t n) {
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
  static constexpr ::size_t length(const char_type *s) {
    ::size_t res = 0;
    while ((*s) != eof()) {
      s++;
      res++;
    }
    return res;
  }
  static constexpr const char_type *find(const char_type *s, ::size_t n, const char_type &c) {
    if (n == 0) return nullptr;
    while ((*s) != eof() && !eq((*s), c))s++;
    return s;
  }
  static constexpr char_type *move(char_type *s1, const char_type *s2, ::size_t n) {
    if (n == 0) return s1;
    return static_cast<char_type *>(memmove(s1, s2, n));
  }
  static constexpr char_type *copy(char_type *s1, const char_type *s2, ::size_t n) {
    if (n == 0)return s1;
    return static_cast<char_type *>(memcpy(s1, s2, n));
  }
  static constexpr char_type to_char_type(const int_type &c) noexcept { return static_cast<char_type>(c); }
  static constexpr int_type to_int_type(const char_type &c) noexcept { return static_cast<int_type>(c); }
  static constexpr bool eq_int_type(const int_type &c1, const int_type &c2) noexcept { return c1 == c2; }
  static constexpr char_type *assign(char_type *s, ::size_t n, char_type a) {
    if (n == 0)return s;
    return static_cast<char_type *>(memset(s, a, n));
  }
};
}

#endif //MIRAI_INCLUDE_MIRAI_DEFS_TYPES_H_
