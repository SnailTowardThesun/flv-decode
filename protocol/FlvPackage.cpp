//
// Created by Hank on 2016/3/2.
//

#include "FlvPackage.h"

FlvPackage::~FlvPackage()
{
	if(payload_data_ != nullptr) delete[] payload_data_;
}

FlvPackage::FlvPackage():previous_packet_size_(0),
	packet_type_(18),
	payload_size_{0},
	time_stamp_lower_{0},
	time_stamp_upper_(0),
	stream_id_{0},
	payload_data_(nullptr)
{

}

bool FlvPackage::decode_one_flv_package(const char* data, int data_size, FlvPacketPayloadOperation opera)
{
	std::cout<<"ready to decode one flv-package"<<std::endl;
	if(data_size < 15) 
	{
		std::cout<<"the flv-package's size is error"<<std::endl;
		return false;
	}
	// previous package'size 
	std::string message;
	int temp = 0;
	char *ptemp = (char*)&temp,*p=NULL;
	p = (char*)data;
	for(int i = 0; i < 4; i++)
	{
		ptemp[3-i] = p[i];
	}
	std::cout<<"the previous package's size is "<<temp<<std::endl;
	previous_packet_size_ = temp;
	//packet type
	switch(data[4])
	{
	case 0x12:
		message = "RTMP message ID";
		break;
	case 0x09:
		message = "video payload";
		break;
	case 0x08:
		message = "audio payload";
		break;
	default:
		std::cout<<"the packet's type is error"<<std::endl;
		return false;
	}
	std::cout<<"the packet's type is "<<message<<std::endl;
	packet_type_ = data[4];
	// payload size
	p = (char*)&data[5];
	memset(ptemp,0,4);
	for(int i = 0; i < 3; i++)
	{
		ptemp[2-i] = p[i];
		payload_size_[i] = p[i];
	}
	std::cout<<"the size of packet's payload is "<<temp<<std::endl;

	// time stamp lower
	p = (char*)&data[8];
	memset(ptemp,0,4);
	for(int i = 0; i < 3; i++)
	{
		ptemp[2-i] = p[i];
		time_stamp_lower_[i] = p[i];
	}
	std::cout<<"the packet's lower time stamp is "<<temp<<std::endl;
	
	//time stamp upper
	time_stamp_upper_ = data[11];
	std::cout<<"the packet's upper time stamp is "<<(int)time_stamp_upper_<<std::endl;
	
	//stream id
	p = (char*)&data[12];
	memset(ptemp,0,4);
	for(int i = 0; i < 3; i++)
	{
		ptemp[2-i] = p[i];
		stream_id_[i] = p[i];
	}
	std::cout<<"the packet's stream id is "<<temp<<std::endl;

	// payload.
	switch(opera)
	{
	case DOING_NOTHING:
		break;
	case COPY_TO_MEMORY:
		memset(ptemp,0,4);
		ptemp = (char*)payload_size_;
		payload_data_ = new char[temp];
		memcpy(payload_data_,&data[15],temp);
		break;
	}
	return true;
}

int32_t FlvPackage::get_payload_size()
{
	int ret = 0;
	char* pret = (char*)&ret;
	for(int i = 0; i < 3; i++)
	{
		pret[2-i] = payload_size_[i];
	}
	return ret;
}

const char* FlvPackage::get_payload()
{
	return (const char*)payload_data_;
}
