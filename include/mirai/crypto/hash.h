//
// Created by Arendellian13 on 2022/5/1.
//

#ifndef MIRAI_INCLUDE_MIRAI_CRYPTO_HASH_H_
#define MIRAI_INCLUDE_MIRAI_CRYPTO_HASH_H_

#include "mirai/defs/types.h"
#include "mirai/utils/bytearray.h"
#include "mirai/utils/encode.h"

namespace OpenSSL {
#include <openssl/md5.h>
}

namespace mirai::crypto {
inline mirai::utils::ByteArray MD5(mirai::utils::ByteArray plain) {
  byte md5_digest[MD5_DIGEST_LENGTH];
  const byte *buf = (byte *) (plain.data());
  OpenSSL::MD5(buf, plain.length(), md5_digest);
  mirai::utils::ByteArray res((md5_digest), MD5_DIGEST_LENGTH);
  return res;
}
}

#endif //MIRAI_INCLUDE_MIRAI_CRYPTO_HASH_H_
