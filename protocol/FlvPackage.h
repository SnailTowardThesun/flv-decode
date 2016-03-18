//
// Created by Hank on 2016/3/2.
//

#ifndef FLV_DECODE_FLVPACKAGE_H
#define FLV_DECODE_FLVPACKAGE_H

#include "../kernel/FlvCommonHeaderFile.h"

static const long DEFAULT_FLV_PACKAGE_SIZE = 15;
typedef enum _enFlvPacketPayloadOperation
{
    DOING_NOTHING = 0,
    COPY_TO_MEMORY,
}FlvPacketPayloadOperation;

class FlvPackage 
{
public:
    FlvPackage();
    virtual ~FlvPackage();
private:
    unsigned int previous_packet_size_;
    unsigned char packet_type_;
    char payload_size_[3];
    char time_stamp_lower_[3];
    unsigned char time_stamp_upper_;
    char stream_id_[3];
    char* payload_data_;
public:
    bool decode_one_flv_package(const char* data, int data_size,FlvPacketPayloadOperation operate);

    int32_t get_payload_size();
    const char *get_payload();
};


#endif //FLV_DECODE_FLVPACKAGE_H
