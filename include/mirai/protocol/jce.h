#include "mirai/defs/types.h"
#include "mirai/defs/macros.h"
#include "mirai/utils/bytearray.h"

#include <memory>
#include <utility>
#include <memory>

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
  TYPE_STRUCT = 10,
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

struct JceHead {
  JceType type;
  std::uint8_t tag;
};

// 所有Jce元素的基类
struct JceField {
  virtual const JceType getType() = 0;
};

struct ZeroField;
struct Int8Field;
struct Int16Field;
struct Int32Field;
struct Int64Field;
struct StringField;
struct FloatField;
struct DoubleField;
struct SimpleListField;
struct ListField;
struct MapField;
struct StructField;

using FieldPtr = std::shared_ptr<JceField>;
using ZeroFieldPtr = std::shared_ptr<ZeroField>;
using Int8FieldPtr = std::shared_ptr<Int8Field>;
using Int16FieldPtr = std::shared_ptr<Int16Field>;
using Int32FieldPtr = std::shared_ptr<Int32Field>;
using Int64FieldPtr = std::shared_ptr<Int64Field>;
using StringFieldPtr = std::shared_ptr<StringField>;
using FloatFieldPtr = std::shared_ptr<FloatField>;
using DoubleFieldPtr = std::shared_ptr<DoubleField>;
using SimpleListFieldPtr = std::shared_ptr<SimpleListField>;
using ListFieldPtr = std::shared_ptr<ListField>;
using MapFieldPtr = std::shared_ptr<MapField>;
using StructFieldPtr = std::shared_ptr<StructField>;

// 封装了一个类保存JceBody
class JceBody {
 public:
  JceBody() { this->field_ptr_ = nullptr; };
  JceBody(const JceBody &rhs) { this->field_ptr_ = rhs.field_ptr_; }
  explicit JceBody(FieldPtr field_ptr) : field_ptr_(std::move(field_ptr)) {}
  const JceType getType() const { return this->field_ptr_->getType(); }

  ZeroFieldPtr getZeroFieldPtr() const { return std::static_pointer_cast<ZeroField>(this->field_ptr_); }
  Int8FieldPtr getInt8FieldPtr() const { return std::static_pointer_cast<Int8Field>(this->field_ptr_); }
  Int16FieldPtr getInt16FieldPtr() const { return std::static_pointer_cast<Int16Field>(this->field_ptr_); }
  Int32FieldPtr getInt32FieldPtr() const { return std::static_pointer_cast<Int32Field>(this->field_ptr_); }
  Int64FieldPtr getInt64FieldPtr() const { return std::static_pointer_cast<Int64Field>(this->field_ptr_); }
  StringFieldPtr getStringFieldPtr() const { return std::static_pointer_cast<StringField>(this->field_ptr_); }
  FloatFieldPtr getFloatFieldPtr() const { return std::static_pointer_cast<FloatField>(this->field_ptr_); }
  DoubleFieldPtr getDoubleFieldPtr() const { return std::static_pointer_cast<DoubleField>(this->field_ptr_); }
  SimpleListFieldPtr getSimpleListFieldPtr() const { return std::static_pointer_cast<SimpleListField>(this->field_ptr_); }
  ListFieldPtr getListFieldPtr() const { return std::static_pointer_cast<ListField>(this->field_ptr_); }
  MapFieldPtr getMapFieldPtr() const { return std::static_pointer_cast<MapField>(this->field_ptr_); }
  StructFieldPtr getStructFieldPtr() const { return std::static_pointer_cast<StructField>(this->field_ptr_); }
  FieldPtr getRawPtr() const { return this->field_ptr_; }

 private:
  FieldPtr field_ptr_;
};

bool operator==(const JceBody &lhs, const JceBody &rhs);
std::size_t JceBodyHashFunc(const JceBody &x);

struct JceElement {
  JceHead head;
  JceBody body;
};

}

namespace std {
template<>
struct hash<mirai::protocol::Jce::JceBody> {
  std::size_t operator()(const mirai::protocol::Jce::JceBody &x) const noexcept {
    return mirai::protocol::Jce::JceBodyHashFunc(x);
  }
};
}

namespace mirai::protocol::Jce {

// 定义Jce里面List和Map的底层容器
using ListType = std::vector<JceBody>;
using MapType = std::unordered_map<JceBody, JceBody>;

}

namespace std {

// 定义hash函数使得可以使用unordered_map
template<>
struct hash<mirai::protocol::Jce::MapType> {
  std::size_t operator()(const mirai::protocol::Jce::MapType &x) {
    auto map_size = sizeof(x);
    using ResType = std::size_t;
    ResType res = 0;
    auto *ptr = (ResType *) (std::addressof(x));
    ResType *const end = ptr + map_size / sizeof(ResType);
    const uint32_t tail = map_size % sizeof(ResType);
    const uint32_t shift = (sizeof(ResType) - sizeof(uint32_t)) << 3;
    uint64_t seed = 131;
    for (; ptr < end; ptr++)res = res * seed + (*ptr);
    if (tail) {
#if BYTEORDER_ENDIAN == BYTEORDER_LITTLE_ENDIAN
      res = res * seed + ((*ptr) & ((1ull << (tail << 3)) - 1));
#elif BYTEORDER_ENDIAN == BYTEORDER_BIG_ENDIAN
      res = res * seed+ ((*ptr) >> ((sizeof(uint64_t) - tail) << 3));
#endif
    }
    return res;
  }
};
template<>
struct hash<mirai::protocol::Jce::ListType> {
  std::size_t operator()(const mirai::protocol::Jce::ListType &x) {
    // TODO: rewrite this hash function
    auto list_size = sizeof(x);
    using ResType = std::size_t;
    ResType res = 0;
    auto *ptr = (ResType *) (std::addressof(x));
    ResType *const end = ptr + list_size / sizeof(ResType);
    const uint32_t tail = list_size % sizeof(ResType);
    const uint32_t shift = (sizeof(ResType) - sizeof(uint32_t)) << 3;
    uint64_t seed = 131;
    for (; ptr < end; ptr++)res = res * seed + (*ptr);
    if (tail) {
#if BYTEORDER_ENDIAN == BYTEORDER_LITTLE_ENDIAN
      res = res * seed + ((*ptr) & ((1ull << (tail << 3)) - 1));
#elif BYTEORDER_ENDIAN == BYTEORDER_BIG_ENDIAN
      res = res * seed+ ((*ptr) >> ((sizeof(uint64_t) - tail) << 3));
#endif
    }
    return res;
  }
};
}

namespace mirai::protocol::Jce {

// Jce的元素的定义
struct ZeroField : public JceField {
  const JceType getType() override { return TYPE_ZERO; }
  static const constexpr std::int8_t value = 0;
};

struct Int8Field : public JceField {
  const JceType getType() override { return TYPE_INT8; }
  std::int8_t value = 0;
};

struct Int16Field : public JceField {
  const JceType getType() override { return TYPE_INT16; }
  std::int16_t value = 0;
};

struct Int32Field : public JceField {
  const JceType getType() override { return TYPE_INT32; }
  std::int32_t value = 0;
};

struct Int64Field : public JceField {
  const JceType getType() override { return TYPE_INT64; }
  std::int64_t value = 0;
};

struct StringField : public JceField {
  const JceType getType() override { return TYPE_STRING1; }
  std::string value;
};

struct FloatField : public JceField {
  const JceType getType() override { return TYPE_FLOAT; }
  float value = 0.0f;
};

struct DoubleField : public JceField {
  const JceType getType() override { return TYPE_DOUBLE; }
  double value = 0.0f;
};

struct SimpleListField : public JceField {
  const JceType getType() override { return TYPE_SIMPLE_LIST; }
  mirai::utils::ByteArray value;
};

struct ListField : public JceField {
  const JceType getType() override { return TYPE_LIST; }
  ListType value;
};

struct MapField : public JceField {
  const JceType getType() override { return TYPE_MAP; }
  MapType value;
};

struct StructField : public JceField {
  const JceType getType() override { return TYPE_STRUCT; }
  JceBody value[256];
};

struct StructEndField : public JceField {
  const JceType getType() override { return TYPE_STRUCT_END; }
};

static const auto kStructEndSymbol = JceBody(std::make_shared<StructEndField>());

JceBody createZeroField() {
  return JceBody(std::static_pointer_cast<JceField>(std::make_shared<ZeroField>()));
}

JceBody createInt8Field(std::int8_t x) {
  auto ptr = std::make_shared<Int8Field>();
  ptr->value = x;
  return JceBody(ptr);
}

JceBody createInt16Field(std::int16_t x) {
  auto ptr = std::make_shared<Int16Field>();
  ptr->value = x;
  return JceBody(ptr);
}

JceBody createInt32Field(std::int32_t x) {
  auto ptr = std::make_shared<Int32Field>();
  ptr->value = x;
  return JceBody(ptr);
}

JceBody createInt64Field(std::int64_t x) {
  auto ptr = std::make_shared<Int64Field>();
  ptr->value = x;
  return JceBody(ptr);
}

JceBody createFloatField(float x) {
  auto ptr = std::make_shared<FloatField>();
  ptr->value = x;
  return JceBody(ptr);
}

JceBody createDoubleField(double x) {
  auto ptr = std::make_shared<DoubleField>();
  ptr->value = x;
  return JceBody(ptr);
}

JceBody createStringField(const std::string &str) {
  auto ptr = std::make_shared<StringField>();
  ptr->value = str;
  return JceBody(ptr);
}

JceBody createSimpleListField(const utils::ByteArray &buf) {
  auto ptr = std::make_shared<SimpleListField>();
  ptr->value = buf;
  return JceBody(ptr);
}

JceHead readHead(utils::ByteStream &bs);
JceBody readBody(utils::ByteStream &bs, JceType type);
JceBody readStruct(utils::ByteStream &bs);
JceElement readElement(utils::ByteStream &bs);
JceBody decode(utils::ByteArray &buf);

// Convert a JceBody to a JSON-style string
std::string jceBodyToString(const JceBody &pack);

}

namespace mirai {
inline std::ostream &operator<<(std::ostream &os, const protocol::Jce::JceBody &pack) {
  return os << protocol::Jce::jceBodyToString(pack);
}
}

namespace std {
template<>
struct hash<mirai::protocol::Jce::StructField> {
  std::size_t operator()(const mirai::protocol::Jce::StructField &x) {
    // TODO: rewrite this hash function
    auto list_size = sizeof(x);
    using ResType = std::size_t;
    ResType res = 0;
    auto *ptr = (ResType *) (std::addressof(x));
    ResType *const end = ptr + list_size / sizeof(ResType);
    const uint32_t tail = list_size % sizeof(ResType);
    const uint32_t shift = (sizeof(ResType) - sizeof(uint32_t)) << 3;
    uint64_t seed = 131;
    for (; ptr < end; ptr++)res = res * seed + (*ptr);
    if (tail) {
#if BYTEORDER_ENDIAN == BYTEORDER_LITTLE_ENDIAN
      res = res * seed + ((*ptr) & ((1ull << (tail << 3)) - 1));
#elif BYTEORDER_ENDIAN == BYTEORDER_BIG_ENDIAN
      res = res * seed+ ((*ptr) >> ((sizeof(uint64_t) - tail) << 3));
#endif
    }
    return res;
  }
};
}
