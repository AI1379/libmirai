#include <gtest/gtest.h>

#include "mirai/utils/bytearray.h"

#include <string>

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
