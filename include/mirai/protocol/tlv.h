#ifndef MIRAI_INCLUDE_MIRAI_UTILS_TLV_H_
#define MIRAI_INCLUDE_MIRAI_UTILS_TLV_H_


namespace mirai::utils {

class Writer
{
private:
    /* data */
public:
    Writer(/* args */);
    ~Writer();

    Writer& writeU8();
    Writer& writeU16();
    Writer& write32();
    Writer& writeU32();
    Writer& writeU64();
    Writer& writeBytes();
    
};

Writer::Writer(/* args */)
{
}

Writer::~Writer()
{
}



}


#endif MIRAI_INCLUDE_MIRAI_UTILS_TLV_H_
