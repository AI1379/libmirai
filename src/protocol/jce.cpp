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

JceBody readBody(utils::ByteStream &bs, JceType type) {
  std::size_t len;
  using mirai::operator>>;
  switch (type) {
    case TYPE_ZERO:return createZeroField();
    case TYPE_INT8: {
      int8_t int8Res;
      bs >> (int8_t &) (int8Res);
      return createInt8Field(int8Res);
    }
    case TYPE_INT16: {
      int16_t int16Res;
      bs >> (int16_t &) (int16Res);
      return createInt16Field(int16Res);
    }
    case TYPE_INT32: {
      int32_t int32Res;
      bs >> (int32_t &) (int32Res);
      return createInt32Field(int32Res);
    }
    case TYPE_INT64: {
      int64_t int64Res;
      bs >> (int64_t &) (int64Res);
      return createInt64Field(int64Res);
    }
    case TYPE_FLOAT: {
      uint32_t floatRes;
      bs >> (uint32_t &) (floatRes);
      return createFloatField(*((float *) (&floatRes)));
    }
    case TYPE_DOUBLE: {
      uint64_t doubleRes;
      bs >> (uint64_t &) (doubleRes);
      return createDoubleField(*((double *) (&doubleRes)));
    }
    case TYPE_STRING1:
    case TYPE_STRING4: {
      if (type == TYPE_STRING1)bs >> (uint8_t &) (len);
      else bs >> (uint32_t &) (len);
      return createStringField(utils::toString(utils::readLen(bs, len)));
    }
    case TYPE_SIMPLE_LIST: {
      readHead(bs);
      // TODO: confirm the size of SimpleList
      len = readElement(bs).body.getInt32FieldPtr()->value;
      return createSimpleListField(utils::readLen(bs, len));
    }
    case TYPE_MAP: {
      len = readElement(bs).body.getInt32FieldPtr()->value;
      MapFieldPtr res = std::make_shared<MapField>();
      while (len--) {
        res->value[readElement(bs).body] = readElement(bs).body;
      }
      return JceBody(res);
    }
    case TYPE_LIST: {
      len = readElement(bs).body.getInt32FieldPtr()->value;
      ListFieldPtr res = std::make_shared<ListField>();
      while (len--) {
        res->value.push_back(readElement(bs).body);
      }
      return JceBody(res);
    }
    case TYPE_STRUCT: {
      return readStruct(bs);
    }
    case TYPE_STRUCT_END: {
      return kStructEndSymbol;
    }
  }
}

}