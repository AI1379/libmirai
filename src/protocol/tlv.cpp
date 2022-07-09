#include "mirai/protocol/tlv.h"

#include "mirai/crypto/random.h"
#include "mirai/utils/date.h"
#include "mirai/utils/encode.h"
#include "mirai/core/device.h"
#include "mirai/crypto/hash.h"
#include "mirai/crypto/tea.h"

#include "mirai/protocol/pb/data.pb.h"

namespace mirai::protocol::TLV {

using namespace mirai::utils;
using namespace mirai::crypto;

utils::ByteArray tlv01(std::uint32_t uin) {
  utils::ByteStream bs;
  bs << 1_u16
     << Random::randomBytes(4)
     << uin
     << std::int32_t(date::now() & 0xffffffff)
     << ByteArray(4, 0_byte) // ip
     << 0_u16;
  return concat(0x01_u16, std::uint16_t(bs.str().length()), bs.str());
}

utils::ByteArray tlv08() {
  return concat(0x08_u16, std::uint16_t(8), hexDecode("0000000008040000"));
}
utils::ByteArray tlv16(const utils::ByteArray &guid) {
  utils::ByteStream bs;
  auto apk = core::device::getApkInfo(core::device::AndroidWatch);
  bs << 7_u32
     << apk.appid
     << apk.subid
     << guid
     << withShortLength(toByteArray(apk.id))
     << withShortLength(toByteArray(apk.sort_version))
     << withShortLength(apk.sign);
  return concat(0x16_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv18(std::uint32_t appid, std::uint32_t uin) {
  utils::ByteStream bs;
  bs << 1_u16 << 1536_u32 << appid << 0_u32 << uin << 0_u32;
  return concat(0x18_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv1B() {
  utils::ByteStream bs;
  bs << 0_u32 << 0_u32 << 3_u32 << 4_u32 << 72_u32 << 2_u32 << 2_u32 << 0_u32;
  return concat(0x1B_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv1D() {
  utils::ByteStream bs;
  bs << 1_byte << 184024956_u32 << 0_u32 << 0_byte << 0_u32;
  return concat(0x1D_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv1F() {
  utils::ByteStream bs;
  bs << 0_byte
     << withShortLength(toByteArray("android"))
     << withShortLength(toByteArray("7.1.2"))
     << 2_u16
     << withShortLength(toByteArray("China Mobile GSM"))
     << withShortLength(ByteArray(0, 0_byte))
     << withShortLength(toByteArray("wifi"));
  return concat(0x1f_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv33(const ByteArray &guid) {
  return concat(0x33_u16, std::uint16_t(guid.length()), guid);
}
utils::ByteArray tlv35() {
  return concat(0x35_u16, 4_u16, 8_u32);
}
utils::ByteArray tlv100(const core::device::Apk &apk, int emp) {
  utils::ByteStream bs;
  bs << 1_u16
     << 7_u32 // SSO Version
     << apk.appid
     << (emp ? 2 : apk.subid)
     << 0_u32
     << apk.sigmap;
  return concat(0x100_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv104(const ByteArray &sig) {
  return concat(0x104_u16, std::uint16_t(sig.length()), sig);
}
utils::ByteArray tlv106(const ByteArray &md5pass,
                        const core::device::Apk &apk,
                        const ByteArray &tgtgt,
                        const ByteArray &guid,
                        std::uint32_t uin) {
  utils::ByteStream bs;
  bs << 4_u16 // tgtgt version
     << Random::randomBytes(4)
     << 7_u32 // SSO version
     << apk.appid
     << 0_u32 // app client version
     << std::uint64_t(uin)
     << std::int32_t(date::now() & 0xffffffff)
     << ByteArray(4, 0_byte) // fake ip
     << 1_byte // save password
     << md5pass
     << tgtgt
     << 0_u32
     << 1_byte // guid available
     << guid
     << apk.subid
     << 1_u32 // login type password
     << withShortLength(toByteArray(std::to_string(uin)))
     << 0_u16;
  ByteArray key = MD5(concat(md5pass, ByteArray(4, 0_byte), uin));
  ByteArray res = tea::encrypt(bs.str(), key);
  return concat(0x106_u16, std::uint16_t(res.length()), res);
}
utils::ByteArray tlv107() {
  return concat(0x107_u16, 6_u16,
                0_u16, // pic type
                0_byte, // captcha type
                0_u16, // pic size
                1_byte // ret type
  );
}
utils::ByteArray tlv109(const std::string &imei) {
  ByteArray imei_hash = MD5(toByteArray(imei));
  return concat(0x109_u16, std::uint16_t(imei_hash.length()), imei_hash);
}
utils::ByteArray tlv10A(const ByteArray &tgt) {
  return concat(0x10a_u16, std::uint16_t(tgt.length()), tgt);
}
utils::ByteArray tlv116(std::uint32_t bitmap) {
  utils::ByteStream bs;
  bs << 0_byte
     << bitmap
     << 0x10400_u32 // sub sigmap
     << 1_byte // size of app list
     << 1600000226_u32; // app id list[0]
  return concat(0x116_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv124(const core::device::FullDevice &device) {
  utils::ByteStream bs;
  bs << withTlvLimitedLength(toByteArrayView(device.os_type), 16)
     << withTlvLimitedLength(toByteArrayView(device.version.release), 16)
     << 2_u16 // Network type
     << withTlvLimitedLength(toByteArrayView(device.sim), 16)
     << 0_u16
     << withTlvLimitedLength(toByteArrayView(device.apn), 16);
  return concat(0x124_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv128(const core::device::FullDevice &device) {
  utils::ByteStream bs;
  bs << 0_u16
     << 0_byte
     << 1_byte
     << 0_byte
     << 16777216_u32
     << withTlvLimitedLength(toByteArrayView(device.model), 32)
     << withTlvLimitedLength(device.guid, 16)
     << withTlvLimitedLength(toByteArrayView(device.brand), 16);
  return concat(0x128_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv141(const core::device::FullDevice &device) {
  utils::ByteStream bs;
  bs << 1_u16 // ver
     << withShortLength(toByteArrayView(device.sim))
     << 2_u16 // Network type
     << withShortLength(toByteArrayView(device.apn));
  return concat(0x141_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv142(const std::string &id) {
  utils::ByteStream bs;
  bs << 0_u16
     << withTlvLimitedLength(toByteArrayView(id), 32);
  return concat(0x142_u16, std::uint16_t(bs.str().length()), bs.str());;
}
utils::ByteArray tlv143(const ByteArray &d2) {
  return concat(0x143_u16, std::uint16_t(d2.length()), d2);
}
utils::ByteArray tlv144(const ByteArray &tgtgt, const core::device::FullDevice &device) {
  utils::ByteStream bs;
  bs << 5_u16
     << tlv109(device.imei)
     << tlv52D(device)
     << tlv124(device)
     << tlv128(device)
     << tlv16E(device.model);
  ByteArray res = tea::encrypt(bs.str(), tgtgt);
  return concat(0x144_u16, std::uint16_t(res.length()), res);
}
utils::ByteArray tlv145(const utils::ByteArray &guid) {
  return concat(0x145_u16, std::uint16_t(guid.length()), guid);
}
utils::ByteArray tlv147(const core::device::Apk &apk) {
  utils::ByteStream bs;
  bs << apk.appid
     << withTlvLimitedLength(toByteArrayView(apk.sort_version), 5)
     << withShortLength(apk.sign);
  return concat(0x147_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv154(std::uint32_t seq) {
  return concat(0x154_u16, 4_u16, std::uint32_t(seq + 1));
}
utils::ByteArray tlv16E(const std::string &model) {
  return concat(0x16e_u16, std::uint16_t(model.length()), model);
}
utils::ByteArray tlv174(const utils::ByteArray &t174) {
  return concat(0x174_u16, std::uint16_t(t174.length()), t174);
}
utils::ByteArray tlv177(const core::device::Apk &apk) {
  utils::ByteStream bs;
  bs << 0x01_byte
     << std::uint32_t(apk.buildtime)
     << withShortLength(toByteArrayView(apk.sdkver));
  return concat(0x177_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv17A() {
  return concat(0x17a_u16, 4_u16, 9_u32);
}
utils::ByteArray tlv17C(const std::string &code) {
  ByteArray res = withShortLength(toByteArrayView(code));
  return concat(0x17c_u16, std::uint16_t(res.length()), res);
}
utils::ByteArray tlv187(const std::string &mac_address) {
  ByteArray res = MD5(toByteArray(mac_address));
  return concat(0x187_u16, std::uint16_t(res.length()), res);
}
utils::ByteArray tlv188(const std::string &android_id) {
  ByteArray res = MD5(toByteArray(android_id));
  return concat(0x188_u16, std::uint16_t(res.length()), res);
}
utils::ByteArray tlv191() {
  return concat(0x191_u16, 1_u16, 0x82_byte);
}
utils::ByteArray tlv193(const std::string &ticket) {
  return concat(0x193_u16, std::uint16_t(ticket.length()), ticket);
}
utils::ByteArray tlv194(const ByteArray &imsi) {
  return concat(0x194_u16, std::uint16_t(imsi.length()), imsi);
}
utils::ByteArray tlv197() {
  ByteArray res = withShortLength(ByteArray(1, 0_byte));
  return concat(0x197_u16, std::uint16_t(res.length()), res);
}
utils::ByteArray tlv198() {
  ByteArray res = withShortLength(ByteArray(1, 0_byte));
  return concat(0x198_u16, std::uint16_t(res.length()), res);
}
utils::ByteArray tlv202(const core::device::FullDevice &device) {
  utils::ByteStream bs;
  bs << withTlvLimitedLength(toByteArrayView(device.wifi_bssid), 16)
     << withTlvLimitedLength(toByteArrayView(device.wifi_ssid), 32);
  return concat(0x202_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv400(std::uint32_t uin, const ByteArray &guid) {
  utils::ByteStream bs;
  bs << 1_u16
     << std::uint64_t(uin)
     << guid
     << Random::randomBytes(16)
     << 1_s32
     << 16_s32
     << std::uint32_t(date::now() & 0xffffffff)
     << ByteArray(0, 0_byte);
  return concat(0x400_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv401() {
  return concat(0x401_u16, 16_u16, Random::randomBytes(16));
}
utils::ByteArray tlv511() {
  utils::ByteStream bs;
  bs << std::uint16_t(network::kDomains.size());
  for (const std::string &domain : network::kDomains) {
    bs << 0x01_byte << withShortLength(toByteArrayView(domain));
  }
  return concat(0x511_u16, std::uint16_t(bs.str().length()), bs.str());
}
utils::ByteArray tlv516() {
  return concat(0x516_u16, 4_u16, 0_u32);
}
utils::ByteArray tlv521() {
  return concat(0x521_u16, 6_u16,
                0_u32, // product type
                0_u16 // const
  );
}
utils::ByteArray tlv525() {
  ByteArray tmp = concat(1_u16, 0x536_u16, withShortLength(hexDecode("0100")));
  return concat(0x525_u16, std::uint16_t(tmp.length()), tmp);
}
utils::ByteArray tlv52D(const core::device::FullDevice &device) {
  // TODO: Protobuf
  utils::ByteArray device_pb = core::device::genFullDeviceProtobuf(device);
  return concat(0x52d_u16, std::uint16_t(device_pb.length()), device_pb);
}

}