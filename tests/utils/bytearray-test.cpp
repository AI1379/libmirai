#include <gtest/gtest.h>

#include "mirai/utils/bytearray.h"

#include <string>
#include <iostream>

using mirai::utils::ByteStream;
using mirai::operator<<;
using mirai::operator>>;

TEST(MiraiUtilTest, ByteStreamOutputTest1) {
  ByteStream bs;
  std::string str("Love you 3000.");
  bs << str;
//  str = ("\n3000...That\'s crazy.");
  bs << "\n3000...That\'s crazy.";
  std::string exp_res("Love you 3000.\n3000...That\'s crazy.");
  auto res = mirai::utils::toString(bs.str());
  ASSERT_EQ(res, exp_res);
}

TEST(MiraiUtilTest, ByteStreamOutputTest2) {
  ByteStream bs;
  bs << 0x41424344;
  auto res = mirai::utils::toString(bs.str());
  ASSERT_EQ(res, std::string("ABCD"));
}

TEST(MiraiUtilTest, ByteStreamOutputTest3) {
  ByteStream bs;
  bs << mirai::byte(0x41) << int16_t(0x4243) << int32_t(0x44454647);
  auto res = mirai::utils::toString(bs.str());
  ASSERT_EQ(res, std::string("ABCDEFG"));
}

TEST(MiraiUtilTest, ByteStreamInputTest1) {
  ByteStream bs;
  mirai::utils::ByteArray inp = mirai::utils::toByteArray("ABCD");
  bs.str(inp);
  mirai::byte res;
  bs >> res;
  ASSERT_EQ(res, 0x41);
}

TEST(MiraiUtilTest, ByteStreamInputTest2) {
  ByteStream bs;
  mirai::utils::ByteArray inp = mirai::utils::toByteArray("ABCDEFG");
  bs.str(inp);
  mirai::utils::ByteArray res = mirai::utils::readLen(bs, 2);
  ASSERT_EQ(mirai::utils::toString(res), "AB");
  res = mirai::utils::readLen(bs, 3);
  ASSERT_EQ(mirai::utils::toString(res), "CDE");
  mirai::utils::ByteArray exp_res = mirai::utils::toByteArray("");
  ASSERT_EQ(mirai::utils::readLen(bs, 0), exp_res);
}

TEST(MiraiUtilTest, ByteStreamTest1) {
  ByteStream bs;
//  mirai::utils::ByteArray inp = mirai::utils::toByteArray("ABCD");
//  bs.str(inp);
  bs << "ABCD";
  mirai::byte res1;
  bs >> (mirai::byte &) (res1);
  ASSERT_EQ(res1, 0x41);
  bs << "EFG";
  uint32_t res2;
  bs >> res2;
  ASSERT_EQ(res2, 0x42434445);
}

TEST(MiraiUtilTest, ByteStreamTest2) {
  ByteStream bs;
  bs << "ABCD";
  ASSERT_EQ(mirai::utils::readableLen(bs), 4);
  mirai::byte x;
  bs >> (mirai::byte &) (x);
  ASSERT_EQ(x, (uint8_t) ('A'));
  ASSERT_EQ(mirai::utils::readableLen(bs), 3);
}

TEST(MiraiUtilTest, ConcatTest1) {
  auto res = mirai::utils::concat("One for the money, two for the show. ",
                                  "I never was ready, so I watched you go.");
  mirai::utils::ByteArray exp_res =
      mirai::utils::toByteArray("One for the money, two for the show. I never was ready, so I watched you go.");
  ASSERT_EQ(res, exp_res);
}

TEST(MiraiUtilTest, ConcatTest2) {
  auto res = mirai::utils::concat("You were drivin' the getaway car\n",
                                  "We were flyin', but we'd never get far\n",
                                  "Don't pretend it's such a mystery\n",
                                  "Think about the place where you first met me");
  mirai::utils::ByteArray exp_res = mirai::utils::toByteArray(
      R"(You were drivin' the getaway car
We were flyin', but we'd never get far
Don't pretend it's such a mystery
Think about the place where you first met me)");
  ASSERT_EQ(res,exp_res);
}
