//
// Created by Hank on 2016/3/2.
//

#ifndef FLV_DECODE_FLVHEADER_H
#define FLV_DECODE_FLVHEADER_H

#include "../kernel/FlvCommonHeaderFile.h"

static const long DEFAULT_FLV_HEADER_SIZE = 9;
enum FlvFileType
{
    VIDEO_ONLY = 0x01,
    AUDIO_ONLY = 0x04,
    VIDEO_AND_AUDIO = 0x05,
};

class FlvHeader
{
public:
    FlvHeader();
    FlvHeader(FlvFileType type);
    virtual ~FlvHeader();
private:
    char signature_[3];
    unsigned char version_;
    unsigned char flags_;
    unsigned long header_size_;
public:
    void set_file_type(FlvFileType type);
    FlvFileType get_file_type();
    bool decode_flv_header(const char* header, int header_size);
    static long  get_default_header_size(){return 9;}
};

#endif //FLV_DECODE_FLVHEADER_H
