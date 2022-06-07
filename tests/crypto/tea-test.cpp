//
// Created by Arendellian13 on 2022/5/1.
//

#include <gtest/gtest.h>

#include "mirai/crypto/tea.h"

#include "mirai/utils/bytearray.h"
#include "mirai/utils/encode.h"

using namespace mirai::utils;

TEST(MiraiCryptoTest, TeaEncryptTest1) {
  ByteArray data = toByteArray("To be or not to be, that is a question");
  ByteArray key = hexDecode("4033b4a6beda50e743a4ea3cbfa80956");
  ByteArray res = mirai::crypto::tea::encrypt(data, key);
  ByteArray exp_res =
      hexDecode("6b57f0afdac5c4258344541e609045ea9d08944a3243c3f168d5c3fd95d1d61df779121ba481c2c9f2b168cdcca2c095");
  ASSERT_EQ(res, exp_res);
}

TEST(MiraiCryptoTest, TeaDecryptTest1) {
  ByteArray data =
      hexDecode("6b57f0afdac5c4258344541e609045ea9d08944a3243c3f168d5c3fd95d1d61df779121ba481c2c9f2b168cdcca2c095");
  ByteArray key = hexDecode("4033b4a6beda50e743a4ea3cbfa80956");
  ByteArray res = mirai::crypto::tea::decrypt(data, key);
  ByteArray exp_res = toByteArray("To be or not to be, that is a question");
  ASSERT_EQ(res, exp_res);
}
