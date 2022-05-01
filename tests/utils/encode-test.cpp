//
// Created by Arendellian13 on 2022/4/30.
//

#include <gtest/gtest.h>

#include "mirai/utils/encode.h"

#include <string>
#include <iostream>

using std::string;
using namespace mirai::utils;

TEST(MiraiUtilTest, Base64EncodeTest1) {
  ByteArray inp = toByteArray("Only an act of true love can thaw a frozen heart.");
  string exp_res = "T25seSBhbiBhY3Qgb2YgdHJ1ZSBsb3ZlIGNhbiB0aGF3IGEgZnJvemVuIGhlYXJ0Lg==";
  string res = base64Encode(inp);
  ASSERT_EQ(res, exp_res);
}

TEST(MiraiUtilTest, Base64DecodeTest1) {
  string inp = "T25seSBhbiBhY3Qgb2YgdHJ1ZSBsb3ZlIGNhbiB0aGF3IGEgZnJvemVuIGhlYXJ0Lg==";
  ByteArray exp_res = toByteArray("Only an act of true love can thaw a frozen heart.");
  ByteArray res = base64Decode(inp);
  ASSERT_EQ(res, exp_res);
}

TEST(MiraiUtilTest, Base64EncodeTest2) {
  ByteArray inp = toByteArray("When all is lost, then all is found.");
  string exp_res = "V2hlbiBhbGwgaXMgbG9zdCwgdGhlbiBhbGwgaXMgZm91bmQu";
  string res = base64Encode(inp);
  ASSERT_EQ(res, exp_res);
}

TEST(MiraiUtilTest, Base64DecodeTest2) {
  string inp = "V2hlbiBhbGwgaXMgbG9zdCwgdGhlbiBhbGwgaXMgZm91bmQu";
  ByteArray exp_res = toByteArray("When all is lost, then all is found.");
  ByteArray res = base64Decode(inp);
  ASSERT_EQ(res, exp_res);
}

TEST(MiraiUtilTest, HexEncodeTest1) {
  ByteArray inp = toByteArray("The cold never bother me anyway.");
  string exp_res = "54686520636f6c64206e6576657220626f74686572206d6520616e797761792e";
  string res = hexEncode(inp);
  ASSERT_EQ(res, exp_res);
}

TEST(MiraiUtilTest, HexDecodeTest1) {
  string inp = "54686520636f6c64206e6576657220626f74686572206d6520616e797761792e";
  ByteArray exp_res = toByteArray("The cold never bother me anyway.");
  ByteArray res = hexDecode(inp);
  ASSERT_EQ(res, exp_res);
}

TEST(MiraiUtilTest, HexEncodeTest2) {
  ByteArray inp = toByteArray("Do you wanna build a snowman?");
  string exp_res = "446f20796f752077616e6e61206275696c64206120736e6f776d616e3f";
  string res = hexEncode(inp);
  ASSERT_EQ(res, exp_res);
}

TEST(MiraiUtilTest, HexDecodeTest2) {
  string inp = "446f20796f752077616e6e61206275696c64206120736e6f776d616e3f";
  ByteArray exp_res = toByteArray("Do you wanna build a snowman?");
  ByteArray res = hexDecode(inp);
  ASSERT_EQ(res, exp_res);
}

