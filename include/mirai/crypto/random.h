
#ifndef MIRAI_INCLUDE_MIRAI_CRYPTO_RANDOM_H_
#define MIRAI_INCLUDE_MIRAI_CRYPTO_RANDOM_H_

#include "mirai/utils/bytearray.h"

#include <random>
#include <memory>

namespace mirai::crypto {
class Random {
 private:
  std::random_device rd_;
  std::mt19937 mt_;
  Random() {
    mt_ = std::mt19937(rd_());
  }
  ~Random() {}
 public:
  static Random &getInstance() {
    static Random instance;
    return instance;
  }
  static byte randomByte() {
    static std::uniform_int_distribution dist(0, 256);
    return dist(Random::getInstance().mt_);
  }
  static utils::ByteArray randomBytes(std::size_t len) {
    utils::ByteArray res;
    for (std::size_t idx = 0; idx < len; idx++) {
      res.push_back(Random::randomByte());
    }
    return res;
  }
};
}

#endif //MIRAI_INCLUDE_MIRAI_CRYPTO_RANDOM_H_
