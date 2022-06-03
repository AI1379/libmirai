
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

struct ShortDevice {
  std::string product, device, board, brand, model, wifi_ssid, bootloader, android_id, boot_id, proc_version,
      mac_address, ip_address, imei, incremental;
};
struct FullDevice;

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
  res.wifi_ssid = "TP-LINK-"+std::to_string(uin);
  res.bootloader = "U-boot";
  return res;
}

}

#endif //MIRAI_INCLUDE_MIRAI_CORE_DEVICE_H_
