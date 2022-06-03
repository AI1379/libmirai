#include <gtest/gtest.h>

#include "mirai/protocol/jce.h"

#include "mirai/utils/encode.h"

#include <iostream>

TEST(MiraiProtocolTest, JceDecodeTest1) {
  mirai::utils::ByteArray buf = mirai::utils::hexDecode(
      "10022C3C4201B2CD43560353534F6613537663526571436865636B41707049444E65777D0000320800010603726573180001061A4B5151436F6E6669672E4772617955696E436865636B526573701D0000070A1C2C3D000C0B8C980CA80C");
  auto decoded = mirai::protocol::Jce::decode(buf);
  auto decoded_string = mirai::protocol::Jce::jceBodyToString(decoded);
  auto exp_res = R"({"1":2,"2":0,"3":0,"4":28495171,"5":"SSO","6":"SvcReqCheckAppIDNew","7":base64://CAABBgNyZXMYAAEGGktRUUNvbmZpZy5HcmF5VWluQ2hlY2tSZXNwHQAABwocLD0ADA==,"8":0,"9":{},"10":{},})";
  ASSERT_EQ(decoded_string,exp_res);
}

TEST(MiraiProtocolTest, JceDecodeTest2) {
  mirai::utils::ByteArray buf = mirai::utils::base64Decode(
      "EA0sNhNBdmVuZ2VycywgYXNzZW1ibGUuRwAAAbtPaCwgSSdsbCBwaWNrIHlvdSB1cApJZiB5b3UgY29tZSBiYWNrIHRvIEFtZXJpY2EsIGp1c3QgaGl0IG1lIHVwCidDYXVzZSB0aGlzIGlzIGNyYXp5IGxvdmUsIEknbGwgY2F0Y2ggeW91IG9uIHRoZSBmbGlwIHNpZGUKSWYgeW91IGNvbWUgYmFjayB0byBDYWxpZm9ybmlhLCB5b3Ugc2hvdWxkIGp1c3QgaGl0IG1lIHVwCldlJ2xsIGRvIHdoYXRldmVyIHlvdSB3YW50LCB0cmF2ZWwgd2hlcmV2ZXIgaG93IGZhcgpXZSdsbCBoaXQgdXAgYWxsIHRoZSBvbGQgcGxhY2VzCldlJ2xsIGhhdmUgYSBwYXJ0eSwgd2UnbGwgZGFuY2UgJ3RpbCBkYXduCkknbGwgcGljayB1cCBhbGwgb2YgeW91ciBWb2d1ZXMgYW5kIGFsbCBvZiB5b3VyIFJvbGxpbmcgU3RvbmVzCllvdXIgZmF2b3JpdGUgbGlxdW9yIG9mZiB0aGUgdG9wIHNoZWxmCkknbGwgdGhyb3cgYSBwYXJ0eSBhbGwgbmlnaHQgbG9uZ1J/////aAAEBgZGcm96ZW4RB90GB1RhbmdsZWQRB9oGBU1vYW5hEQfgBgdFbmNhbnRvEQfleQAEBgdJcm9uTWFuBgRUaG9yBg5DYXB0YWluQW1lcmljYQYIQXZlbmdlcnONAAEAzVNheSB5b3UnbGwgcmVtZW1iZXIgbWUKU3RhbmRpbicgaW4gYSBuaWNlIGRyZXNzClN0YXJpbicgYXQgdGhlIHN1bnNldCwgYmFiZQpSZWQgbGlwcyBhbmQgcm9zeSBjaGVla3MKU2F5IHlvdSdsbCBzZWUgbWUgYWdhaW4KRXZlbiBpZiBpdCdzIGp1c3QgaW4geW91cgpXaWxkZXN0IGRyZWFtcywgYWgsIGhhIChIYSwgaGEpCldpbGRlc3QgZHJlYW1zLCBhaCwgaGE=");
  auto decoded = mirai::protocol::Jce::decode(buf);
  auto decoded_string = mirai::protocol::Jce::jceBodyToString(decoded);
  std::cout << decoded_string << std::endl;
  /* Possible output:
   * {"1":13,"2":0,"3":"Avengers, assemble.","4":"Oh, I'll pick you up
If you come back to America, just hit me up
'Cause this is crazy love, I'll catch you on the flip side
If you come back to California, you should just hit me up
We'll do whatever you want, travel wherever how far
We'll hit up all the old places
We'll have a party, we'll dance 'til dawn
I'll pick up all of your Vogues and all of your Rolling Stones
Your favorite liquor off the top shelf
I'll throw a party all night long","5":2147483647,"6":{"Moana":2016,"Tangled":2010,"Encanto":2021,"Frozen":2013,},"7":["IronMan","Thor","CaptainAmerica","Avengers",],"8":base64://U2F5IHlvdSdsbCByZW1lbWJlciBtZQpTdGFuZGluJyBpbiBhIG5pY2UgZHJlc3MKU3RhcmluJyBhdCB0aGUgc3Vuc2V0LCBiYWJlClJlZCBsaXBzIGFuZCByb3N5IGNoZWVrcwpTYXkgeW91J2xsIHNlZSBtZSBhZ2FpbgpFdmVuIGlmIGl0J3MganVzdCBpbiB5b3VyCldpbGRlc3QgZHJlYW1zLCBhaCwgaGEgKEhhLCBoYSkKV2lsZGVzdCBkcmVhbXMsIGFoLCBoYQ==,}
   */
}
