//
// Created by Arendellian13 on 2022/5/1.
//

#include <gtest/gtest.h>

#include "mirai/crypto/ecdh.h"

using mirai::utils::hexDecode;

TEST(MiraiCryptoTest, EcdhContainerTest1) {
  mirai::crypto::ECDH::ECDHContainer testContainer(NID_X9_62_prime256v1);
  auto originalPrivateKey(hexDecode("d6bb247828b5be0de504e6a48c952997f0f575c7f9b63ed586c210c0d35cef75"));
  testContainer.setPrivateKey(originalPrivateKey);
  auto answerPublicKey(hexDecode(
      "0455e570a1a7b3c47743b1c51d6d6d7c0c95865015beeafe359fce4739641cf361a53305e049f8ec48e1d812ec1965121d94b8d20bb3bde1b0041e6983497c52e5"));
  auto publicKey = testContainer.getPublicKey();
  auto privateKey = testContainer.getPrivateKey();
  ASSERT_EQ(answerPublicKey, publicKey);
  ASSERT_EQ(originalPrivateKey, privateKey);
}

TEST(MiraiCryptoTest, EcdhContainerTest2) {
  mirai::crypto::ECDH::ECDHContainer testContainer(NID_X9_62_prime256v1);
  auto originalPrivateKey(hexDecode("d6bb247828b5be0de504e6a48c952997f0f575c7f9b63ed586c210c0d35cef75"));
  testContainer.setPrivateKey(originalPrivateKey);
  auto testPeerKey = mirai::crypto::kOicqPublicKey;
  auto sharedKey = testContainer.computeSecret(testPeerKey);
  auto ansSharedKey = hexDecode("8f23b12acdf4786986204034a02b147334fc04a69b8eed16c8f0f1b35ce3cbf3");
  ASSERT_EQ(ansSharedKey, sharedKey);
}

TEST(MiraiCryptoTest, EcdhContainerTest3) {
  mirai::crypto::ECDH::ECDHContainer testContainer(NID_X9_62_prime256v1);
  auto originalPrivateKey(hexDecode("fe8e61d5053cbbaae593c5d68b9be1b2a3a39c5b883ead711f15aeeb8555a542"));
  testContainer.setPrivateKey(originalPrivateKey);
  auto testPeerKey = mirai::crypto::kOicqPublicKey;
  auto sharedKey = testContainer.computeSecret(testPeerKey);
  auto ansSharedKey = hexDecode("e0c9a4f65e0b025bbb9d89573d1f632b18574a5e16531796350ae7d56a33e51e");
  ASSERT_EQ(ansSharedKey, sharedKey);
}

TEST(MiraiCryptoTest, EcdhTest1) {
  mirai::crypto::ECDH testEcdh;
  mirai::utils::ByteArray private_key = hexDecode("72576416e00d38084b533516aa2df9f12796d41859be20cfaa9b6742a0f72aa3");
  testEcdh.setPrivateKey(private_key);
  auto shared_key = testEcdh.getShareKey();
  auto exp_shared_key = hexDecode("5fed7883e0018bd04d40762e920fac11");
  ASSERT_EQ(shared_key, exp_shared_key);
}

