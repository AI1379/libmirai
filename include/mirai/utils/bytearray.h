//
// Created by Arendellian13 on 2022/4/30.
//

#ifndef MIRAI_INCLUDE_MIRAI_UTILS_BYTEARRAY_H_
#define MIRAI_INCLUDE_MIRAI_UTILS_BYTEARRAY_H_

#include "mirai/defs/types.h"
#include "mirai/utils/encode.h"

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

}

#endif //MIRAI_INCLUDE_MIRAI_UTILS_BYTEARRAY_H_
