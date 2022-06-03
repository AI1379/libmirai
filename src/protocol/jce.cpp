#include "mirai/protocol/jce.h"

#include "mirai/utils/encode.h"

namespace mirai::protocol::Jce {

bool operator==(const JceBody &lhs, const JceBody &rhs) {
  if (lhs.getType() != rhs.getType()) return false;
  if (lhs.getRawPtr() == rhs.getRawPtr() && lhs.getRawPtr() == nullptr)return true;
  switch (lhs.getType()) {
    case TYPE_INT8:return (lhs.getInt8FieldPtr()->value == rhs.getInt8FieldPtr()->value);
    case TYPE_INT16:return (lhs.getInt16FieldPtr()->value == rhs.getInt16FieldPtr()->value);
    case TYPE_INT32:return (lhs.getInt32FieldPtr()->value == rhs.getInt32FieldPtr()->value);
    case TYPE_INT64:return (lhs.getInt64FieldPtr()->value == rhs.getInt64FieldPtr()->value);
    case TYPE_FLOAT:return (lhs.getFloatFieldPtr()->value == rhs.getFloatFieldPtr()->value);
    case TYPE_DOUBLE:return (lhs.getDoubleFieldPtr()->value == rhs.getDoubleFieldPtr()->value);
    case TYPE_STRING1:
    case TYPE_STRING4:return (lhs.getStringFieldPtr()->value == rhs.getStringFieldPtr()->value);
      // TODO: complete comparison of Map, List and Struct
    case TYPE_MAP: {
      auto x = lhs.getMapFieldPtr()->value;
      auto y = rhs.getMapFieldPtr()->value;
      return x == y;
    }
    case TYPE_LIST: {
      return lhs.getListFieldPtr()->value == rhs.getListFieldPtr()->value;
    }
    case TYPE_STRUCT: {
      auto x = lhs.getStructFieldPtr()->value;
      auto y = rhs.getStructFieldPtr()->value;
      for (std::size_t idx = 0; idx < 256; idx++) {
        if (x[idx] != y[idx])return false;
      }
      return true;
    }
    case TYPE_SIMPLE_LIST: {
      return lhs.getSimpleListFieldPtr()->value == rhs.getSimpleListFieldPtr()->value;
//      if (lhs.getSimpleListFieldPtr()->value.length() != rhs.getSimpleListFieldPtr()->value.length()) {
//        return false;
//      }
//      auto x = lhs.getSimpleListFieldPtr()->value;
//      auto y = rhs.getSimpleListFieldPtr()->value;
//      for (auto itx = x.begin(), ity = y.end(); itx != x.end() && ity != y.end(); itx++, ity++) {
//        if ((*itx) != (*ity))return false;
//      }
//      return true;
    }
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
    case TYPE_STRING1:return hash<decltype(x.getStringFieldPtr()->value)>{}(x.getStringFieldPtr()->value);
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

static std::size_t readJceLen(utils::ByteStream &bs) {
  auto x = readElement(bs).body;
  switch (x.getType()) {
    case TYPE_ZERO:return 0;
    case TYPE_INT8:return static_cast<std::size_t>(x.getInt8FieldPtr()->value);
    case TYPE_INT16:return static_cast<std::size_t>(x.getInt16FieldPtr()->value);
    case TYPE_INT32:return static_cast<std::size_t>(x.getInt32FieldPtr()->value);
    case TYPE_INT64:return static_cast<std::size_t>(x.getInt64FieldPtr()->value);
    default:throw std::logic_error("Invalid length type");
  }
}

JceBody readBody(utils::ByteStream &bs, JceType type) {
  std::size_t len = 0;
//  using mirai::operator>>;
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
    case TYPE_STRING1: {
      bs >> (uint8_t &) (len);
      return createStringField(utils::toString(utils::readLen(bs, len)));
    }
    case TYPE_STRING4: {
      bs >> (uint32_t &) (len);
      return createStringField(utils::toString(utils::readLen(bs, len)));
    }
    case TYPE_SIMPLE_LIST: {
      readHead(bs);
      len = readJceLen(bs);
//      len = readElement(bs).body.getInt8FieldPtr()->value;
      return createSimpleListField(utils::readLen(bs, len));
    }
    case TYPE_MAP: {
//      len = readElement(bs).body.getInt8FieldPtr()->value;
      len = readJceLen(bs);
      MapFieldPtr res = std::make_shared<MapField>();
      JceBody x, y;
      while (len--) {
        x = readElement(bs).body;
        y = readElement(bs).body;
        res->value.insert(std::make_pair(x, y));
//        res->value[readElement(bs).body] = readElement(bs).body;
      }
      return JceBody(res);
    }
    case TYPE_LIST: {
      len = readJceLen(bs);
//      len = readElement(bs).body.getInt32FieldPtr()->value;
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

JceBody readStruct(utils::ByteStream &bs) {
  StructFieldPtr ptr = std::make_shared<StructField>();
  while (utils::readableLen(bs)) {
    auto res = readElement(bs);
    if (res.body == kStructEndSymbol) {
      return JceBody(ptr);
    } else {
      ptr->value[res.head.tag] = res.body;
    }
  }
  throw std::logic_error("Invalid JCE Package");
}

JceElement readElement(utils::ByteStream &bs) {
  JceElement res;
  res.head = readHead(bs);
  res.body = readBody(bs, res.head.type);
  return res;
}

JceBody decode(utils::ByteArray &buf) {
  utils::ByteStream bs;
  bs.str(buf);
  StructFieldPtr res_ptr = std::make_shared<StructField>();
  JceElement elem;
  while (utils::readableLen(bs)) {
    elem = readElement(bs);
    res_ptr->value[elem.head.tag] = elem.body;
  }
  return JceBody(res_ptr);
}

std::string jceBodyToString(const JceBody &pack) {
  std::ostringstream oss;
  switch (pack.getType()) {
    case TYPE_INT8: {
      return std::to_string(pack.getInt8FieldPtr()->value);
    }
    case TYPE_INT16: {
      return std::to_string(pack.getInt16FieldPtr()->value);
    }
    case TYPE_INT32: {
      return std::to_string(pack.getInt32FieldPtr()->value);
    }
    case TYPE_INT64: {
      return std::to_string(pack.getInt64FieldPtr()->value);
    }
    case TYPE_FLOAT: {
      return std::to_string(pack.getFloatFieldPtr()->value);
    }
    case TYPE_DOUBLE: {
      return std::to_string(pack.getDoubleFieldPtr()->value);
    }
    case TYPE_STRING1:
    case TYPE_STRING4: {
      oss << "\"" << pack.getStringFieldPtr()->value << "\"";
      return oss.str();
    }
    case TYPE_MAP: {
      oss << "{";
      for (const auto &x : pack.getMapFieldPtr()->value) {
        oss << jceBodyToString(x.first) << ":" << jceBodyToString(x.second) << ",";
      }
      oss << "}";
      return oss.str();
    }
    case TYPE_LIST: {
      oss << "[";
      for (const auto &x : pack.getListFieldPtr()->value) {
        oss << jceBodyToString(x) << ",";
      }
      oss << "]";
      return oss.str();
    }
    case TYPE_STRUCT: {
      oss << "{";
      auto raw_struct = pack.getStructFieldPtr()->value;
      for (std::size_t idx = 0; idx < 256; idx++) {
        if (raw_struct[idx].getRawPtr() != nullptr) {
          oss << "\"" << idx << "\":" << jceBodyToString(raw_struct[idx]) << ",";
        }
      }
      oss << "}";
      return oss.str();
    }
    case TYPE_STRUCT_END: {
      return "JCE_STRUCT_END_SYMBOL";
    }
    case TYPE_ZERO: {
      return "0";
    }
    case TYPE_SIMPLE_LIST: {
      oss << "base64://" << utils::base64Encode(pack.getSimpleListFieldPtr()->value);
      return oss.str();
    }
  }
}

utils::ByteArray createElement(const std::uint8_t &tag, const JceBody &pack) {
  utils::ByteStream bs;
  if (tag < 15) bs << (mirai::byte) (static_cast<uint8_t>(pack.getType()) | (tag << 4));
  else bs << (mirai::byte) (static_cast<uint8_t>(pack.getType()) | 0xf0) << (mirai::byte) (tag);
  switch (pack.getType()) {
    case TYPE_INT8: {
      bs << (pack.getInt8FieldPtr()->value);
      break;
    }
    case TYPE_INT16: {
      bs << (pack.getInt16FieldPtr()->value);
      break;
    }
    case TYPE_INT32: {
      bs << (pack.getInt32FieldPtr()->value);
      break;
    }
    case TYPE_INT64: {
      bs << (pack.getInt64FieldPtr()->value);
      break;
    }
    case TYPE_FLOAT: {
      bs << utils::ByteArray(reinterpret_cast<mirai::byte *>(&(pack.getFloatFieldPtr()->value)), 4);
      break;
    }
    case TYPE_DOUBLE: {
      bs << utils::ByteArray(reinterpret_cast<mirai::byte *>(&(pack.getDoubleFieldPtr()->value)), 8);
      break;
    }
    case TYPE_STRING1:
    case TYPE_STRING4: {
      auto str = pack.getStringFieldPtr()->value;
      if (str.length() < 256) {
        bs << (std::uint8_t) (str.length()) << str;
      } else {
        bs.seekp(0, std::ios::beg);
        if (tag < 15) bs << (mirai::byte) (static_cast<uint8_t>(TYPE_STRING4) | (tag << 4));
        else bs << (mirai::byte) (static_cast<uint8_t>(TYPE_STRING4) | 0xf0) << (mirai::byte) (tag);
        bs << (std::uint64_t) (str.length()) << str;
      }
      break;
    }
    case TYPE_MAP: {
      bs << createElement(kLengthTag, createIntField(pack.getMapFieldPtr()->value.size()));
      for (auto &x : pack.getMapFieldPtr()->value) {
        bs << createElement(kMapKeyTag, x.first) << createElement(kMapValueTag, x.second);
      }
      break;
    }
    case TYPE_LIST: {
      bs << createElement(kLengthTag, createIntField(pack.getListFieldPtr()->value.size()));
      for (auto &x : pack.getListFieldPtr()->value) {
        bs << createElement(kListElementTag, x);
      }
      break;
    }
    case TYPE_STRUCT: {
      auto raw_struct = pack.getStructFieldPtr()->value;
      for (std::size_t idx = 0; idx < 256; idx++) {
        if (raw_struct[idx].getRawPtr() != nullptr) {
          bs << createElement(idx, raw_struct[idx]);
        }
      }
      bs << createElement(kStructEndTag, kStructEndSymbol);
      break;
    }
    case TYPE_STRUCT_END:
    case TYPE_ZERO: break;
    case TYPE_SIMPLE_LIST: {
      bs << (mirai::byte) (kBytesTag)
         << createElement(kLengthTag, createIntField(pack.getSimpleListFieldPtr()->value.length()))
         << pack.getSimpleListFieldPtr()->value;
    }
  }
  return bs.str();
}

}