#ifndef MIRAI_INCLUDE_MIRAI_UTILS_TLV_H_
#define MIRAI_INCLUDE_MIRAI_UTILS_TLV_H_

#include "mirai/utils/bytearray.h"
#include "mirai/core/device.h"

#include <vector>
#include <string>

namespace mirai::network {
const std::vector<std::string> kDomains = {
    "aq.qq.com",
    "buluo.qq.com",
    "connect.qq.com",
    "docs.qq.com",
    "game.qq.com",
    "gamecenter.qq.com",
    // "graph.qq.com",
    "haoma.qq.com",
    "id.qq.com",
    // "imgcache.qq.com",
    "kg.qq.com",
    "mail.qq.com",
    "mma.qq.com",
    "office.qq.com",
    // "om.qq.com",
    "openmobile.qq.com",
    "qqweb.qq.com",
    "qun.qq.com",
    "qzone.qq.com",
    "ti.qq.com",
    "v.qq.com",
    "vip.qq.com",
    "y.qq.com",
};
}

namespace mirai::protocol::TLV {

inline utils::ByteArray withLongLength(const utils::ByteArrayView &v) {
  return utils::concat(std::uint32_t(v.length() + 4), v);
}

inline utils::ByteArray withShortLength(const utils::ByteArrayView &v) {
  return utils::concat(std::uint16_t(v.length()), v);
}

inline utils::ByteArray withTlvLimitedLength(const utils::ByteArrayView &v, std::size_t limit) {
  if (v.length() <= limit) {
    return withShortLength(v);
  }
  return withShortLength(v.substr(0, limit));
}

utils::ByteArray tlv01(std::uint32_t uin);
utils::ByteArray tlv08();
utils::ByteArray tlv16(const utils::ByteArray &guid);
utils::ByteArray tlv18(std::uint32_t appid, std::uint32_t uin);
utils::ByteArray tlv1B();
utils::ByteArray tlv1D();
utils::ByteArray tlv1F();
utils::ByteArray tlv33(const utils::ByteArray &guid);
utils::ByteArray tlv35();
utils::ByteArray tlv100(const core::device::Apk &apk, int emp = 0);
utils::ByteArray tlv104(const utils::ByteArray &sig);
utils::ByteArray tlv106(const utils::ByteArray &md5pass,
                        const core::device::Apk &apk,
                        const utils::ByteArray &tgtgt,
                        const utils::ByteArray &guid,
                        std::uint32_t uin);
utils::ByteArray tlv107();
utils::ByteArray tlv109(const std::string &imei);
utils::ByteArray tlv10A(const utils::ByteArray &tgt);
utils::ByteArray tlv116(std::uint32_t bitmap);
utils::ByteArray tlv124(const core::device::FullDevice &device);
utils::ByteArray tlv128(const core::device::FullDevice &device);
utils::ByteArray tlv141(const core::device::FullDevice &device);
utils::ByteArray tlv142(const std::string &id);
utils::ByteArray tlv143(const utils::ByteArray &d2);
utils::ByteArray tlv144(const utils::ByteArray &tgtgt, const core::device::FullDevice &device);
utils::ByteArray tlv145(const utils::ByteArray &guid);
utils::ByteArray tlv147(const core::device::Apk &apk);
utils::ByteArray tlv154(std::uint32_t seq);
utils::ByteArray tlv16E(const std::string &model);
utils::ByteArray tlv174(const utils::ByteArray &t174);
utils::ByteArray tlv177(const core::device::Apk &apk);
utils::ByteArray tlv17A();
utils::ByteArray tlv17C(const std::string &code);
utils::ByteArray tlv187(const std::string &mac_address);
utils::ByteArray tlv188(const std::string &android_id);
utils::ByteArray tlv191();
utils::ByteArray tlv193(const std::string &ticket);
utils::ByteArray tlv194(const utils::ByteArray &imsi);
utils::ByteArray tlv197();
utils::ByteArray tlv198();
utils::ByteArray tlv202(const core::device::FullDevice &device);
utils::ByteArray tlv400(std::uint32_t uin, const utils::ByteArray &guid);
utils::ByteArray tlv401();
utils::ByteArray tlv511();
utils::ByteArray tlv516();
utils::ByteArray tlv521();
utils::ByteArray tlv525();
utils::ByteArray tlv52D(const core::device::FullDevice &device);

}

#endif // MIRAI_INCLUDE_MIRAI_UTILS_TLV_H_
