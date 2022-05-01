//
// Created by Arendellian13 on 2022/5/1.
//

#ifndef MIRAI_INCLUDE_MIRAI_CRYPTO_ECDH_H_
#define MIRAI_INCLUDE_MIRAI_CRYPTO_ECDH_H_

#include "mirai/defs/types.h"
#include "mirai/utils/bytearray.h"
#include "mirai/utils/encode.h"
#include "mirai/crypto/hash.h"

namespace OpenSSL {
#include <openssl/ec.h>
#include <openssl/evp.h>
}

#include <exception>
#include <utility>

namespace mirai::crypto {
static const utils::ByteArray kOicqPublicKey = utils::hexDecode(
    "04EBCA94D733E399B2DB96EACDD3F69A8BB0F74224E2B44E3357812211D2E62EFBC91BB553098E25E33A799ADC7F76FEB208DA7C6522CDB0719A305180CC54A82E");

class ECDH {
 public:
  class ECDHContainer {
   public:
    explicit ECDHContainer(int nid);
    ~ECDHContainer();
    utils::ByteArray getPublicKey();
    utils::ByteArray getPrivateKey();
    void setPrivateKey(utils::ByteArray);
    utils::ByteArray computeSecret(utils::ByteArray);
    void generateKeys();
   private:
    OpenSSL::EC_KEY *ecdh_key_;
    OpenSSL::EC_GROUP *ecdh_group_;
    OpenSSL::EC_POINT *ecdh_public_key_;
    OpenSSL::BIGNUM *ecdh_private_key_;
    int nid_;
  };
  struct ECDHComputeError : public std::exception {
    [[nodiscard]] const char *what() const noexcept override { return "Error occurred when computing ECDH Key pair"; }
  };
 private:
  utils::ByteArray share_key_;
  void handleErrors() { throw ECDHComputeError(); }
  ECDHContainer container_;
 public:
  ECDH();
  utils::ByteArray getPublicKey() { return this->container_.getPublicKey(); }
  void setPrivateKey(const utils::ByteArray &key) {
    this->container_.setPrivateKey(key);
    this->share_key_ = MD5(utils::ByteArray(this->container_.computeSecret(kOicqPublicKey), 0, 16));
  }
  utils::ByteArray getShareKey() { return this->share_key_; }
};

}

#endif //MIRAI_INCLUDE_MIRAI_CRYPTO_ECDH_H_
