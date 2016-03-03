//
// Created by Hank on 2016/3/2.
//

#ifndef FLV_DECODE_FLVHEADER_H
#define FLV_DECODE_FLVHEADER_H

#include "../kernel/FlvCommonHeaderFile.h"

static const int32_t DEFAULT_FLV_HEADER_SIZE = 9;
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
    uint8_t version_;
    uint8_t flags_;
    uint32_t header_size_;
public:
    void set_file_type(FlvFileType type);
    FlvFileType get_file_type();
	bool decode_flv_header(const char* header, int header_size);

	static const int32_t get_default_header_size(){return 9;}
};

/*

class FlvHeader
{
    FlvHeader();
 //   FlvHeader(FlvFileType type);
    virtual ~FlvHeader();
private:
    char signature_[3];
    uint8_t version_;
    uint8_t flags_;
    uint32_t header_size_;
public:
  //  void set_file_type(FlvFileType type);
   // FlvFileType get_file_type();
};
*/
#endif //FLV_DECODE_FLVHEADER_H
