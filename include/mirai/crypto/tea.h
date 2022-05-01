//
// Created by Arendellian13 on 2022/4/30.
//

#ifndef MIRAI_INCLUDE_MIRAI_CRYPTO_TES_H_
#define MIRAI_INCLUDE_MIRAI_CRYPTO_TES_H_

#include "mirai/utils/bytearray.h"

#include <utility>
#include <cstdint>
#include <string>
#include <exception>

namespace mirai::crypto {

using std::uint32_t;

constexpr uint32_t deltas[] = {0x9e3779b9, 0x3c6ef372, 0xdaa66d2b, 0x78dde6e4,
                               0x1715609d, 0xb54cda56, 0x5384540f, 0xf1bbcdc8,
                               0x8ff34781, 0x2e2ac13a, 0xcc623af3, 0x6a99b4ac,
                               0x08d12e65, 0xa708a81e, 0x454021d7, 0xe3779b90};

inline std::pair<uint32_t, uint32_t> _encrypt(uint32_t x, uint32_t y, uint32_t k0,
                                              uint32_t k1, uint32_t k2, uint32_t k3) {
  uint32_t aa, bb;
  for (std::size_t i = 0; i < 16; i++) {
    aa = ((y << 4) + k0) ^ (y + deltas[i]) ^ (y / 32 + k1);
    x += aa;
    bb = ((x << 4) + k2) ^ (x + deltas[i]) ^ (x / 32 + k3);
    y += bb;
  }
  return std::make_pair(x, y);
}

inline std::pair<uint32_t, uint32_t> _decrypt(uint32_t x, uint32_t y, uint32_t k0,
                                              uint32_t k1, uint32_t k2, uint32_t k3) {
  uint32_t aa, bb;
  for (std::size_t i = 15; i >= 0 && i <= 15; i--) {
    aa = ((x << 4) + k2) ^ (x + deltas[i]) ^ (x / 32 + k3);
    y = y - aa;
    bb = ((y << 4) + k0) ^ (y + deltas[i]) ^ (y / 32 + k1);
    x = x - bb;
  }
  return std::make_pair(x, y);
}

inline utils::ByteArray encrypt(utils::ByteArray data, utils::ByteArray key) {
  using utils::readUInt32BE;
  using utils::writeUInt32BE;
  uint32_t n = 6 - data.size();
  n = n % 8 + 2;
  utils::ByteArray v;
  v.push_back((byte) ((n - 2) | 0xf8));
  v.insert(v.end(), (size_t) (n), 0);
  v.insert(v.end(), data.begin(), data.end());
  v.insert(v.end(), (size_t) (7), (byte) (0));
  uint32_t k0, k1, k2, k3;
  k0 = readUInt32BE(key, 0);
  k1 = readUInt32BE(key, 4);
  k2 = readUInt32BE(key, 8);
  k3 = readUInt32BE(key, 12);
  uint32_t r1, r2, t1, t2;
  r1 = r2 = t1 = t2 = 0;
  uint32_t a1, a2, b1, b2;
  std::pair<uint32_t, uint32_t> res;
  for (size_t i = 0; i < v.size(); i += 8) {
    a1 = readUInt32BE(v, i);
    a2 = readUInt32BE(v, i + 4);
    b1 = a1 ^ r1;
    b2 = a2 ^ r2;
    res = _encrypt(b1, b2, k0, k1, k2, k3);
    r1 = res.first ^ t1;
    r2 = res.second ^ t2;
    t1 = b1;
    t2 = b2;
    writeUInt32BE(v, i, r1);
    writeUInt32BE(v, i + 4, r2);
  }
  return v;
}

inline utils::ByteArray decrypt(utils::ByteArray encrypted, utils::ByteArray key) {
  if (encrypted.size() % 8 != 0) // TODO: 重新封装这里的exception
    throw std::invalid_argument(
        "length of encrypted data must be a multiple of 8");
  using utils::readUInt32BE;
  using utils::writeUInt32BE;
  uint32_t k0, k1, k2, k3;
  k0 = readUInt32BE(key, 0);
  k1 = readUInt32BE(key, 4);
  k2 = readUInt32BE(key, 8);
  k3 = readUInt32BE(key, 12);
  uint32_t r1, r2, t1, t2, x, y;
  r1 = r2 = t1 = t2 = x = y = 0;
  uint32_t a1, a2, b1, b2;
  std::pair<uint32_t, uint32_t> res;
  for (size_t i = 0; i < encrypted.size(); i += 8) {
    a1 = readUInt32BE(encrypted, i);
    a2 = readUInt32BE(encrypted, i + 4);
    b1 = a1 ^ x;
    b2 = a2 ^ y;
    res = _decrypt(b1, b2, k0, k1, k2, k3);
    x = res.first;
    y = res.second;
    r1 = x ^ t1;
    r2 = y ^ t2;
    t1 = a1;
    t2 = a2;
    writeUInt32BE(encrypted, i, r1);
    writeUInt32BE(encrypted, i + 4, r2);
  }
  for (auto it = encrypted.end() - 7; it != encrypted.end(); it++) {
    if ((*it) != 0) {
      throw std::logic_error("encrypted data is illegal");
    }
  }
  auto beg = encrypted.begin() + (encrypted[0] & 0x07) + 3;
  auto end = encrypted.end() - 7;
  utils::ByteArray ret(beg, end);
  return ret;
}

}

#endif //MIRAI_INCLUDE_MIRAI_CRYPTO_TES_H_
