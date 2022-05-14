#include "mirai/protocol/jce.h"

namespace mirai::protocol::Jce {

bool operator==(const JceBody &lhs, const JceBody &rhs) {
  if (lhs.getType() != rhs.getType()) return false;
  switch (lhs.getType()) {
    case TYPE_INT8:return (lhs.getInt8FieldPtr()->value == rhs.getInt8FieldPtr()->value);
    case TYPE_INT16:return (lhs.getInt16FieldPtr()->value == rhs.getInt16FieldPtr()->value);
    case TYPE_INT32:return (lhs.getInt32FieldPtr()->value == rhs.getInt32FieldPtr()->value);
    case TYPE_INT64:return (lhs.getInt64FieldPtr()->value == rhs.getInt64FieldPtr()->value);
    case TYPE_FLOAT:return (lhs.getFloatFieldPtr()->value == rhs.getFloatFieldPtr()->value);
    case TYPE_DOUBLE:return (lhs.getDoubleFieldPtr()->value == rhs.getDoubleFieldPtr()->value);
    case TYPE_STRING1:
    case TYPE_STRING4:return (lhs.getStringFieldPtr()->value == rhs.getStringFieldPtr()->value);
      // TODO: complete comparison of Map, List, SimpleList and Struct
    case TYPE_MAP:
    case TYPE_LIST:
    case TYPE_STRUCT:
    case TYPE_SIMPLE_LIST:return true;
    case TYPE_STRUCT_END:throw std::runtime_error("Invalid JCE Field");
    case TYPE_ZERO:return true;
  }
}

std::size_t JceBodyHashFunc(const JceBody &x) {
  using std::hash;
  switch (x.getType()) {
    case TYPE_INT8:return hash<decltype(x.getInt8FieldPtr()->value)>{}(x.getInt8FieldPtr()->value);
    case TYPE_INT16:return hash<decltype(x.getInt16FieldPtr()->value)>{}(x.getInt16FieldPtr()->value);
    case TYPE_INT32:return hash<decltype(x.getInt32FieldPtr()->value)>{}(x.getInt32FieldPtr()->value);
    case TYPE_INT64:return hash<decltype(x.getInt64FieldPtr()->value)>{}(x.getInt64FieldPtr()->value);
    case TYPE_FLOAT:return hash<decltype(x.getFloatFieldPtr()->value)>{}(x.getFloatFieldPtr()->value);
    case TYPE_DOUBLE:return hash<decltype(x.getDoubleFieldPtr()->value)>{}(x.getDoubleFieldPtr()->value);
    case TYPE_STRING1:
    case TYPE_STRING4:return hash<decltype(x.getStringFieldPtr()->value)>{}(x.getStringFieldPtr()->value);
    case TYPE_MAP:return hash<decltype(x.getMapFieldPtr()->value)>{}(x.getMapFieldPtr()->value);
    case TYPE_LIST:return hash<decltype(x.getListFieldPtr()->value)>{}(x.getListFieldPtr()->value);
    case TYPE_STRUCT:return hash<StructField>{}(*(x.getStructFieldPtr()));
    case TYPE_STRUCT_END:throw std::logic_error("Invalid Jce Type");
    case TYPE_ZERO:return hash<int8_t>{}(0);
    case TYPE_SIMPLE_LIST:return hash<decltype(x.getSimpleListFieldPtr()->value)>{}(x.getSimpleListFieldPtr()->value);
  }
}

JceHead readHead(utils::ByteStream &bs) {
  mirai::byte x;
  bs >> x;
  JceHead res{};
  res.type = (JceType) (x & 0x0f);
  res.tag = (x & 0xf0) >> 4;
  if (res.tag == 0x0f) {
    bs >> (mirai::byte &) (res.tag);
  }
  return res;
}

}