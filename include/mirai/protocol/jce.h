#include "mirai/defs/types.h"
#include "mirai/utils/bytearray.h"

#include <utility>

namespace mirai::protocol::Jce {

enum JceType {
  TYPE_INT8 = 0,
  TYPE_INT16 = 1,
  TYPE_INT32 = 2,
  TYPE_INT64 = 3,
  TYPE_FLOAT = 4,
  TYPE_DOUBLE = 5,
  TYPE_STRING1 = 6,
  TYPE_STRING4 = 7,
  TYPE_MAP = 8,
  TYPE_LIST = 9,
  TYPE_STRUCT_BEGIN = 10,
  TYPE_STRUCT_END = 11,
  TYPE_ZERO = 12,
  TYPE_SIMPLE_LIST = 13,
};

enum JceTag {
  TAG_MAP_K = 0,
  TAG_MAP_V = 1,
  TAG_LIST_E = 0,
  TAG_BYTES = 0,
  TAG_LENGTH = 0,
  TAG_STRUCT_END = 0,
};

std::pair<byte,byte> readHead(const utils::ByteArray &buf);
utils::ByteArray readBody(const utils::ByteArray &buf,JceType type);


}