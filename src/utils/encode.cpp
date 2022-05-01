//
// Created by Arendellian13 on 2022/4/30.
//

#include "mirai/defs/types.h"
#include "mirai/utils/encode.h"

#include <cstdint>
#include <utility>
#include <iostream>

std::string mirai::utils::base64Encode(const mirai::utils::ByteArray &inp) {
  std::string res;
  std::uint32_t tmp;
  std::size_t idx;
  for (idx = 0; idx + 2 < inp.length(); idx += 3) {
    tmp = (static_cast<byte>(inp[idx]) << 16) |
        (static_cast<byte>(inp[idx + 1]) << 8) |
        (static_cast<byte>(inp[idx + 2]));
    res.push_back(kBase64EncodeTable[(tmp >> 18) & 0x3f]);
    res.push_back(kBase64EncodeTable[(tmp >> 12) & 0x3f]);
    res.push_back(kBase64EncodeTable[(tmp >> 6) & 0x3f]);
    res.push_back(kBase64EncodeTable[(tmp >> 0) & 0x3f]);
  }
  if (idx != inp.length()) {
    tmp = 0;
    switch (inp.length() - idx) {
      case 2:tmp |= (static_cast<byte>(inp[idx + 1]) << 8);
      case 1:tmp |= (static_cast<byte>(inp[idx]) << 16);
    }
    res.push_back(kBase64EncodeTable[(tmp >> 18) & 0x3f]);
    res.push_back(kBase64EncodeTable[(tmp >> 12) & 0x3f]);
    res.push_back(idx - inp.length() == 1 ? kBase64EncodeTable[(tmp >> 6) & 0x3f] : '=');
    res.push_back('=');
  }
  return res;
}

mirai::utils::ByteArray mirai::utils::base64Decode(const std::string &inp) {
  using mirai::utils::ByteArray;
  ByteArray res;
  std::uint32_t tmp;
  std::string::size_type idx;
  for (idx = 0; idx + 3 < inp.length(); idx += 4) {
    tmp = (kBase64DecodeTable[static_cast<byte>(inp[idx])] << 18) |
        (kBase64DecodeTable[static_cast<byte>(inp[idx + 1])] << 12) |
        (kBase64DecodeTable[static_cast<byte>(inp[idx + 2])] << 6) |
        (kBase64DecodeTable[static_cast<byte>(inp[idx + 3])] << 0);
    res.push_back(static_cast<typename ByteArray::value_type>((tmp >> 16) & 0xff));
    if (inp[idx + 2] == '=') break;
    res.push_back(static_cast<typename ByteArray::value_type>((tmp >> 8) & 0xff));
    if (inp[idx + 3] == '=') break;
    res.push_back(static_cast<typename ByteArray::value_type>(tmp & 0xff));
  }
  return res;
}

std::string mirai::utils::hexEncode(const mirai::utils::ByteArray &inp) {
  std::string res;
  for (typename ByteArray::value_type c : inp) {
    res.push_back(kHexEncodeTable[(static_cast<byte>(c) >> 4) & 0x0f]);
    res.push_back(kHexEncodeTable[static_cast<byte>(c) & 0x0f]);
  }
  return res;
}

mirai::utils::ByteArray mirai::utils::hexDecode(const std::string &inp) {
  using mirai::utils::ByteArray;
  ByteArray res;
  std::size_t idx = 0;
  byte x;
  if (inp.length() % 2 != 0) {
    x = kHexDecodeTable[static_cast<byte>(inp[0])];
    res.push_back(static_cast<typename ByteArray::value_type>(x));
    idx++;
  }
  for (; idx < inp.length(); idx += 2) {
    x = (kHexDecodeTable[static_cast<byte>(inp[idx])] << 4) |
        (kHexDecodeTable[static_cast<byte>(inp[idx + 1])]);
    res.push_back(static_cast<typename ByteArray::value_type>(x));
  }
  return res;
}

