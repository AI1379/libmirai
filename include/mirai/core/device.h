
#ifndef MIRAI_INCLUDE_MIRAI_CORE_DEVICE_H_
#define MIRAI_INCLUDE_MIRAI_CORE_DEVICE_H_

#include "mirai/crypto/random.h"
#include "mirai/crypto/hash.h"

#include <string>

namespace mirai::core::device {
std::uint32_t calcSP(const std::string &imei) {
  std::uint32_t sum = 0;
  for (std::size_t i = 0, j; i < imei.length(); i++) {
    if (i % 2 == 1) {
      j = static_cast<std::uint32_t>(imei[i] - '0') * 2;
      sum += (j % 10 + j / 10);
    } else {
      sum += static_cast<std::uint32_t>(imei[i] - '0');
    }
  }
  return (100 - sum) % 10;
}

// TODO: 减少字符串处理
std::string generateImei(std::uint32_t uin) {
  std::stringstream ss;
  std::string uin_str = std::to_string(uin);
  if (uin % 2 == 1) ss << "86";
  else ss << "35";
  std::uint32_t a, b;
  a = uin >> 16;
  b = uin & 0x00ffffff;
  if (a > 9999) a /= 10;
  if (a < 1000) a = std::stoi(uin_str.substr(0, 4));
  while (b > 9999999) b >>= 1;
  if (b < 1000000) b = std::stoi(uin_str.substr(0, 4) + uin_str.substr(0, 3));
  ss << a << 0 << b;
  auto sp = calcSP(ss.str());
  ss << sp;
  return ss.str();
}

enum Protocol {
  AndroidPhone,
  AndroidPad,
  AndroidWatch,
  iPad,
  iMac
};

struct Apk {
  std::string id;
  std::string name;
  std::string version;
  std::string sort_version;
  utils::ByteArray sign;
  uint32 buildtime{};
  uint32 appid{};
  uint32 subid{};
  uint32 bitmap{};
  uint32 sigmap{};
  std::string sdkver;
  Protocol protocol = AndroidPhone;
};

Apk getApkInfo(Protocol protocol) {
  Apk res;
  res.protocol = protocol;
  using utils::hexDecode;
  switch (protocol) {
    case AndroidPhone: {
      res.id = "com.tencent.mobileqq";
      res.name = "A8.4.1.2703aac4";
      res.version = "8.4.1.2703";
      res.sort_version = "8.4.1";
      res.sign = hexDecode("a6b745bf24a2c277527716f6f36eb68d");
      res.buildtime = 1591690260;
      res.appid = 16;
      res.subid = 537064989;
      res.bitmap = 184024956;
      res.sigmap = 34869472;
      res.sdkver = "6.0.0.2428";
      return res;
    }
    case AndroidPad: {
      res.id = "com.tencent.minihd.qq";
      res.name = "A5.8.9.3460";
      res.version = "5.8.9.3460";
      res.sort_version = "5.8.9";
      res.sign = hexDecode("aa3978f41fd96ff9914a669e186474c7");
      res.buildtime = 1595836208;
      res.appid = 16;
      res.subid = 537065549;
      res.bitmap = 150470524;
      res.sigmap = 1970400;
      res.sdkver = "6.0.0.2433";
      return res;
    }
    case AndroidWatch: {
      res.id = "com.tencent.qqlite";
      res.name = "A2.0.5";
      res.version = "2.0.5";
      res.sort_version = "2.0.5";
      res.sign = hexDecode("a6b745bf24a2c277527716f6f36eb68d");
      res.buildtime = 1559564731;
      res.appid = 16;
      res.subid = 537064446;
      res.bitmap = 16252796;
      res.sigmap = 34869472;
      res.sdkver = "6.0.0.236";
      return res;
    }
    case iPad: {
      res.id = "com.tencent.minihd.qq";
      res.name = "A5.8.9.3460";
      res.version = "5.8.9.3460";
      res.sort_version = "5.8.9";
      res.sign = hexDecode("aa3978f41fd96ff9914a669e186474c7");
      res.buildtime = 1595836208;
      res.appid = 16;
      res.subid = 537065739;
      res.bitmap = 150470524;
      res.sigmap = 1970400;
      res.sdkver = "6.0.0.2433";
      return res;
    }
    case iMac: {
      res.id = "com.tencent.minihd.qq";
      res.name = "A5.8.9.3460";
      res.version = "5.8.9.3460";
      res.sort_version = "5.8.9";
      res.sign = hexDecode("aa3978f41fd96ff9914a669e186474c7");
      res.buildtime = 1595836208;
      res.appid = 16;
      res.subid = 537064315;
      res.bitmap = 150470524;
      res.sigmap = 1970400;
      res.sdkver = "6.0.0.2433";
      return res;
    }
  }
}

struct ShortDevice {
  std::string product, device, board, brand, model, wifi_ssid, bootloader, android_id, boot_id, proc_version,
      mac_address, ip_address, imei, incremental;
};
struct FullDevice {
  std::string product, device, board, brand, model, wifi_ssid, bootloader, android_id, boot_id, proc_version,
      mac_address, ip_address, imei, incremental,os_type;
  struct VersionField{
    std::uint32_t incremental;
    std::string release;
    std::string codename;
    std::uint32_t sdk;
  }version;
  std::string sim;
  std::string apn;
  std::string wifi_bssid;
  utils::ByteArray guid;
  utils::ByteArray imsi;
};

ShortDevice generateShortDevice(uint32_t uin) {
  auto hash = crypto::MD5(utils::toByteArray(std::to_string(uin)));
  auto hex = utils::hexEncode(hash);
  ShortDevice res;
  // TODO: 修改设备信息
  res.product = "MAR4S";
  res.device = "HIM188MOE";
  res.board = "MIRAI";
  res.brand = "OICQX";
  res.model = "Konata 2020";
  res.wifi_ssid = "TP-LINK-" + std::to_string(uin);
  res.bootloader = "U-boot";

  return res;
}

}

#endif //MIRAI_INCLUDE_MIRAI_CORE_DEVICE_H_
