//
// Created by Arendellian13 on 2022/5/1.
//

#include<gtest/gtest.h>

#include "mirai/crypto/hash.h"

#include "mirai/utils/bytearray.h"
#include "mirai/utils/encode.h"

TEST(MiraiCryptoTest, MD5Test1) {
  using namespace mirai::utils;
  ByteArray inp = hexDecode("576520646f6e27742074616c6b2061626f7574204272756e6f2e");
  ByteArray exp_res = hexDecode("02c14b81129768c91fa1f834a23d41f7");
  ByteArray res = mirai::crypto::MD5(inp);
  ASSERT_EQ(res, exp_res);
}