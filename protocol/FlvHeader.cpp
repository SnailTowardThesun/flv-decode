//
// Created by Hank on 2016/3/2.
//

#include "FlvHeader.h"

FlvHeader::FlvHeader()
{    
	signature_[0] = 'F';
    signature_[1] = 'L';
    signature_[2] = 'V';

    version_ = 0x01;

    flags_ = VIDEO_AND_AUDIO;
    header_size_ = 9;

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

bool FlvHeader::decode_flv_header(const char* header, int header_size)
{
	if(header_size < 5) 
	{
		std::cout<<"the header's size is too small"<<std::endl;
		return false;
	}
	// signature
	for(int i = 0; i < 2; i++)
	{
		if(header[i] != signature_[i]) 
		{
			std::cout<<"the header's signature is an error"<<std::endl;
			return false;
		}
	}
	//version
	if(header[3] != version_)
	{
		std::cout<<"the header's version is an error"<<std::endl;
		return false;
	}
	// flags
	if(header[4] != 0x01 && header[4] != 0x04 && header[4] != 0x05)
	{
		std::cout<<"the header's flags is an error"<<std::endl;
		return false;
	}
	flags_ = header[4];
	std::string message;
	switch(flags_)
	{
	case VIDEO_ONLY:
		message = "the flv file's type is video only";
		break;
	case AUDIO_ONLY:
		message = "the flv file's type is audio only";
		break;
	case VIDEO_AND_AUDIO:
		message = "the flv file's type is video and audio";
		break;
	}
	std::cout<<message<<std::endl;
	return true;
}
