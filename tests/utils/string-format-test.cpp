#include <gtest/gtest.h>

#include "mirai/utils/bytearray.h"
#include "mirai/utils/string-format.h"

TEST(MiraiUtilTest, StringFormatTest1) {
  std::string str1 = "Elsa";
  const char *str2 = "Anna";
  mirai::utils::ByteArray str3 = mirai::utils::toByteArray("Olaf");
  ASSERT_EQ(mirai::utils::stringFormat("Hello %s, %s and %s", str1, str2, str3), "Hello Elsa, Anna and Olaf");
}
