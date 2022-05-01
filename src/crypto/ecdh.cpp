//
// Created by Arendellian13 on 2022/5/1.
//

#include "mirai/crypto/ecdh.h"

mirai::crypto::ECDH::ECDHContainer::ECDHContainer(int nid)
    : nid_(nid) {
  this->ecdh_group_ = OpenSSL::EC_GROUP_new_by_curve_name(nid);
  if (this->ecdh_group_ == NULL)
    throw ECDHComputeError();
  this->ecdh_key_ = OpenSSL::EC_KEY_new();
  if (this->ecdh_key_ == NULL)
    throw ECDHComputeError();
  if (OpenSSL::EC_KEY_set_group(this->ecdh_key_, this->ecdh_group_) != 1)
    throw ECDHComputeError();
  this->ecdh_public_key_ = OpenSSL::EC_POINT_new(this->ecdh_group_);
  this->ecdh_private_key_ = OpenSSL::BN_new();
  if (this->ecdh_public_key_ == NULL || this->ecdh_private_key_ == NULL)
    throw ECDHComputeError();
}

mirai::crypto::ECDH::ECDHContainer::~ECDHContainer() {
  OpenSSL::EC_GROUP_free(this->ecdh_group_);
  OpenSSL::EC_KEY_free(this->ecdh_key_);
  OpenSSL::EC_POINT_free(this->ecdh_public_key_);
  OpenSSL::BN_free(this->ecdh_private_key_);
}

void mirai::crypto::ECDH::ECDHContainer::generateKeys() {
  if (OpenSSL::EC_KEY_generate_key(this->ecdh_key_) != 1)
    throw ECDHComputeError();
  this->ecdh_public_key_ =
      OpenSSL::EC_POINT_dup(OpenSSL::EC_KEY_get0_public_key(this->ecdh_key_), this->ecdh_group_);
  this->ecdh_private_key_ = OpenSSL::BN_dup(OpenSSL::EC_KEY_get0_private_key(this->ecdh_key_));
  if (this->ecdh_public_key_ == NULL || this->ecdh_private_key_ == NULL)
    throw ECDHComputeError();
}

mirai::crypto::ECDH::ECDH()
    : container_(NID_X9_62_prime256v1) {
  this->container_.generateKeys();
  this->share_key_ =
      mirai::crypto::MD5(mirai::utils::ByteArray(this->container_.computeSecret(mirai::crypto::kOicqPublicKey), 16));
}

void mirai::crypto::ECDH::ECDHContainer::setPrivateKey(mirai::utils::ByteArray private_key) {
  OpenSSL::BIGNUM *private_key_bn = OpenSSL::BN_new();
  OpenSSL::BN_CTX *bn_ctx = OpenSSL::BN_CTX_new();
  OpenSSL::EC_POINT *public_key = OpenSSL::EC_POINT_new(this->ecdh_group_);
  if (public_key == NULL || private_key_bn == NULL || bn_ctx == NULL)
    throw ECDHComputeError();
  if (OpenSSL::BN_bin2bn((byte *) (private_key.data()), private_key.length(), private_key_bn) == NULL)
    throw ECDHComputeError();
  if (OpenSSL::EC_KEY_set_private_key(this->ecdh_key_, private_key_bn) != 1)
    throw ECDHComputeError();
  if (OpenSSL::EC_POINT_mul(this->ecdh_group_, public_key, private_key_bn,
                            NULL, NULL, bn_ctx) != 1)
    throw ECDHComputeError();
  if (OpenSSL::EC_KEY_set_public_key(this->ecdh_key_, public_key) != 1)
    throw ECDHComputeError();
  if (!OpenSSL::EC_KEY_check_key(this->ecdh_key_))
    throw ECDHComputeError();
  this->ecdh_public_key_ = OpenSSL::EC_POINT_dup(public_key, this->ecdh_group_);
  if (this->ecdh_public_key_ == NULL) throw ECDHComputeError();
  this->ecdh_private_key_ = OpenSSL::BN_dup(private_key_bn);
  if (this->ecdh_private_key_ == NULL) throw ECDHComputeError();

  OpenSSL::BN_CTX_free(bn_ctx);
  OpenSSL::EC_POINT_free(public_key);
  OpenSSL::BN_free(private_key_bn);
}

mirai::utils::ByteArray mirai::crypto::ECDH::ECDHContainer::getPublicKey() {
  using namespace OpenSSL;
  BN_CTX *bn_ctx = BN_CTX_new();
  if (bn_ctx == NULL) throw ECDHComputeError();
  byte *buf;
  auto len = EC_POINT_point2buf(this->ecdh_group_,
                                this->ecdh_public_key_,
                                POINT_CONVERSION_UNCOMPRESSED,
                                &buf,
                                bn_ctx);
  if (len == 0) throw ECDHComputeError();
  mirai::utils::ByteArray res(buf, len);
  OPENSSL_free(buf);
  BN_CTX_free(bn_ctx);
  return res;
}

mirai::utils::ByteArray mirai::crypto::ECDH::ECDHContainer::getPrivateKey() {
  using namespace OpenSSL;
  byte *buf;
  auto len = BN_num_bytes(this->ecdh_private_key_);
  buf = (byte *) (malloc(len));
  BN_bn2bin(this->ecdh_private_key_, buf);
  mirai::utils::ByteArray res(buf, len);
  free(buf);
  return res;
}

mirai::utils::ByteArray mirai::crypto::ECDH::ECDHContainer::computeSecret(mirai::utils::ByteArray peer_public_key) {
  using namespace OpenSSL;
  EC_POINT *peer_public_key_point, *shared_key;
  BN_CTX *bn_ctx = BN_CTX_new();
  peer_public_key_point = EC_POINT_new(this->ecdh_group_);
  shared_key = EC_POINT_new(this->ecdh_group_);
  EC_POINT_oct2point(this->ecdh_group_,
                     peer_public_key_point,
                     (byte *) (peer_public_key.data()),
                     peer_public_key.length(),
                     bn_ctx);
  if (peer_public_key_point == NULL) throw ECDHComputeError();
  if (!EC_POINT_mul(this->ecdh_group_, shared_key, NULL,
                    peer_public_key_point, this->ecdh_private_key_, bn_ctx))
    throw ECDHComputeError();
  BN_CTX_free(bn_ctx);
  bn_ctx = BN_CTX_new();
  byte *buf;
  auto len = EC_POINT_point2buf(this->ecdh_group_,
                                shared_key,
                                POINT_CONVERSION_COMPRESSED,
                                &buf,
                                bn_ctx);
  if (len == 0) throw ECDHComputeError();
  mirai::utils::ByteArray res(buf + 1, len - 1);
  OPENSSL_free(buf);
  BN_CTX_free(bn_ctx);
  EC_POINT_free(peer_public_key_point);
  return res;
}

