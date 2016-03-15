//
// Created by Hank on 2016/3/2.
//

#include "FlvDecode.h"
#include "protocol/FlvHttp.h"
#include "protocol/FlvHttp2.h"
FlvDecode::FlvDecode():read_buffer_(nullptr)
{
	read_buffer_ = new char[MAX_SIZE_OF_BUFFER_READ_FROM_FILE];
}

FlvDecode::~FlvDecode()
{
	if(read_buffer_ != nullptr) delete[] read_buffer_;
	if(!flv_package_list_.empty()) flv_package_list_.clear();
}

void FlvDecode::decode_flv_file(std::string filename)
{
    if(filename.empty())
    {
		FlvLog::getInstance()->trace("error","flvdecode.cpp","the file name is empty");
        return;
    }
	std::ifstream file_stream;
	file_stream.open(filename,std::fstream::in);
	// parser the flv header
	file_stream.read(read_buffer_,DEFAULT_FLV_HEADER_SIZE);
	flv_header_.decode_flv_header(read_buffer_,DEFAULT_FLV_HEADER_SIZE);	
	// parser the first package
	int payload_size_read = 0;
	bool ret = false;
	FlvPackage* ppack = new FlvPackage();
	while(file_stream.read(read_buffer_,DEFAULT_FLV_PACKAGE_SIZE))
	{
		if(!(ret = ppack->decode_one_flv_package(read_buffer_,DEFAULT_FLV_PACKAGE_SIZE,DOING_NOTHING))) break;	
		payload_size_read = (int)ppack->get_payload_size();
		file_stream.seekg(payload_size_read,file_stream.cur);
	}
	delete ppack;
	// close the stream
	file_stream.close();
}

bool FlvDecode::parser_url(std::string url, std::string& ip, int& port, std::string& msg)
{
	if(url.empty())
	{
		FlvLog::getInstance()->trace("error","flvdecode.cpp","the url to be parsed is empty");
		return false;
	}
	size_t pos1 = url.find(":",7);
	//get ip
	ip.assign(url.begin() + 7, url.begin()+pos1);
	//get port
	size_t pos2 = url.find("/",pos1);
	std::string port_s;
	port_s.assign(url.begin() + pos1 + 1,url.begin() + pos2);
	port = atoi(port_s.c_str());
	//the last is msg to request
	msg.assign(url.begin() + pos2,url.end());
	return true;
}

void FlvDecode::decode_flv_from_http(std::string url)
{
	// decode the http flv stream;
	if(url.empty()) return;
	FlvHttp2 http_decode;
	std::string ip,msg;
	int port = 8080;
	if(!parser_url(url,ip,port,msg)) return;
	http_decode.initialize(ip,port);
	// send get message 
	//http_decode.send_GET_request("live/livestream.flv");
	http_decode.send_GET_request(msg);
	char* recv_message = new char[1024*1024];
	int32_t recv_message_size = 9;
	// get flv header
	http_decode.get_received_msg(recv_message_size,recv_message);
	if(!flv_header_.decode_flv_header(recv_message, 9)) return;
	// loop to get message and parse
	int payload_size_read = 0;
	bool ret = false;
	FlvPackage* ppack = new FlvPackage();
	do{
		http_decode.get_received_msg(15,recv_message);
		if(!(ret = ppack->decode_one_flv_package(recv_message,DEFAULT_FLV_PACKAGE_SIZE,DOING_NOTHING))) break;
		payload_size_read = (int)ppack->get_payload_size();
	}while(http_decode.get_received_msg(payload_size_read,recv_message));
	
	delete ppack;
	delete[] recv_message;
}
