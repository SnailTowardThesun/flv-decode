//
// Created by Hank on 2016/3/2.
//

#include "FlvHeader.h"

FlvHeader::FlvHeader()
{
    FlvHeader(VIDEO_AND_AUDIO);
}

FlvHeader::FlvHeader(FlvFileType type)
{
    signature_[0] = 'F';

    signature_[1] = 'L';
    signature_[2] = 'V';

    version_ = 0x01;

    flags_ = type;
    header_size_ = 9;
}

FlvHeader::~FlvHeader()
{

}

void FlvHeader::set_file_type(FlvFileType type)
{

}

FlvFileType FlvHeader::get_file_type()
{
    return (FlvFileType)flags_;
}
