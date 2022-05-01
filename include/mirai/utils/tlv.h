#ifndef MIRAI_INCLUDE_MIRAI_UTILS_TLV_H_
#define MIRAI_INCLUDE_MIRAI_UTILS_TLV_H_


namespace mirai::utils {

class Write
{
private:
    /* data */
public:
    Write(/* args */);
    ~Write();

    Write& writeU8();
    Write& writeU16();
    Write& write32();
    Write& writeU32();
    Write& writeU64();
    Write& writeBytes();
    
};

Write::Write(/* args */)
{
}

Write::~Write()
{
}



}


#endif MIRAI_INCLUDE_MIRAI_UTILS_TLV_H_
