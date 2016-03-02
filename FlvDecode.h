//
// Created by Hank on 2016/3/2.
//

#ifndef FLV_DECODE_FLVDECODE_H
#define FLV_DECODE_FLVDECODE_H
#include "./app/FlvLog.h"

class FlvDecode
{
public:
    FlvDecode();
    virtual ~FlvDecode();
public:
    void decode_flv_file(std::string filename);
};


#endif //FLV_DECODE_FLVDECODE_H
