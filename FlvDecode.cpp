//
// Created by Hank on 2016/3/2.
//

#include "FlvDecode.h"
#include "protocol/FlvHttp.h"
FlvDecode::FlvDecode():read_buffer_(nullptr)
{
	read_buffer_ = new char[MAX_SIZE_OF_BUFFER_READ_FROM_FILE];
}

FlvDecode::~FlvDecode()
{
	if(read_buffer_ != nullptr) delete[] read_buffer_;
}

void FlvDecode::decode_flv_file(std::string filename)
{
    if(filename.empty())
    {
        std::cout<<"the file name is empty"<<std::endl;
        return;
    }
    std::cout<<"the name of  file to be decoded is "<<filename<<std::endl;

	std::ifstream file_stream;
	file_stream.open(filename,std::fstream::in);

	// parser the flv header
	file_stream.read(read_buffer_,DEFAULT_FLV_HEADER_SIZE);
	flv_header_.decode_flv_header(read_buffer_,DEFAULT_FLV_HEADER_SIZE);
	
	// parser the first package
	int payload_size_read = 0;
	while(file_stream.read(read_buffer_,DEFAULT_FLV_PACKAGE_SIZE))
	{
		std::cout<<std::endl;
		FlvPackage* ppack = new FlvPackage();
		bool ret = ppack->decode_one_flv_package(read_buffer_,DEFAULT_FLV_PACKAGE_SIZE,DOING_NOTHING);
		
		if(!ret)
		{
			delete ppack;
			break;
		}

		std::cout<<std::endl;
		
		payload_size_read = (int)ppack->get_payload_size();
		file_stream.seekg(payload_size_read,file_stream.cur);
		flv_package_list_.push_back(ppack);
	}
	// close the stream
	file_stream.close();
}

void FlvDecode::decode_flv_from_http(std::string url)
{
	// decode the http flv stream;
	if(url.empty()) return;
	FlvHttp http_decode;
	http_decode.initialize("192.168.9.237",8080);

	// send get message 
	http_decode.send_GET_request("/live/livestream.flv/");
	// loop to get message and parse
	string recv_message;
	while(http_decode.get_received_msg(recv_message))
	{
		std::cout<<recv_message<<endl;
	}

}
