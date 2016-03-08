//
//Created by Hank 2016/3/4
//

#include "FlvHttp.h"

FlvHttp::FlvHttp():decoded_buffer_(nullptr),
	recved_buffer_(nullptr),
	decoded_current_pos_(MAX_LENGTH_OF_RECEIVE_MESSAGE),
	data_left_(0)
{
	decoded_buffer_ = new char[MAX_LENGTH_OF_RECEIVE_MESSAGE];
}

FlvHttp::~FlvHttp()
{
	if(decoded_buffer_ != nullptr) delete[] decoded_buffer_;
}

bool FlvHttp::initialize(string TargetAddr, int32_t TargetPort = 80)
{
	// parse the url 
	
	string url_content;
	if (parse_url(TargetAddr, host_ip_,host_port_,url_content) == 0)
	{
//		cout<<"error to parse the url"<<endl;
	}

	host_ip_ = TargetAddr;
	host_port_ = to_string((int)TargetPort);

	if(!client_socket_.initialize(TargetAddr,TargetPort))
	{
		cout<<"fail to initialize client_socket_"<<endl;
		return false;
	}
	return true;
}

int32_t FlvHttp::parse_url(string url_to_parse, string& targetAddr, string& targetPort, string& requestContent)
{
	if(url_to_parse.empty())
	{
		std::cout<<"the url to be parsed is empty"<<endl;
		return 0;
	}
	// get scheme
	
	// get ip
	
	// get port
	
	// get request
	return requestContent.size();
}

bool FlvHttp::send_GET_request(string msg)
{
	if(msg.empty()) 
	{
		cout<<"the request message to be sended by GET is empty"<<endl;
		return false;
	}
	
	string get_request = "GET "+ msg + " HTTP/1.1\r\n";	
	get_request += "Host: ";
	get_request += host_ip_;
	get_request +=":" + host_port_;
	get_request += "\r\n\r\n";
	
	if(client_socket_.send_msg(get_request, get_request.size()) == 0)
	{
		cout<<"fail to send the request message"<<endl;
		return false;
	}
	return true;
}

bool FlvHttp::send_POST_request(string msg)
{
	if(msg.empty())
	{
		cout<<"the request message to be sended by POST is empty"<<endl;
		return false;
	}
	return true;
}

char* FlvHttp::parse_received_msg(char* msg_to_parse, int32_t& msg_size)
{
	char* msg_result = nullptr;
	if(msg_to_parse == nullptr)
	{
		return nullptr;
	}

	if(msg_to_parse[0] == 'H' && msg_to_parse[1] == 'T' && msg_to_parse[2] == 'T' && msg_to_parse[3] == 'P')
	{

		msg_result = strstr(msg_to_parse, "\r\n\r\n");
		if (msg_result != nullptr)
		{
			msg_result += 4;;
			msg_result = strstr(msg_result, "\r\n");
			msg_result += 2;

		}
	}	
	else 
		msg_result = msg_to_parse;
	return msg_result;
}


int32_t FlvHttp::decode_one_chunked()
{
	if(recved_buffer_ == nullptr) 
	{
		recved_buffer_ = client_socket_.recv_msg(data_left_);
		if(recved_buffer_ == nullptr) return 0;
		decoded_current_pos_ = recved_buffer_;
	}

	int chunk_size = 0;
	char* pchunk_size = (char*)&chunk_size;

	if(decoded_current_pos_[0] == 'H' && decoded_current_pos_[1] == 'T' && decoded_current_pos_[2] == 'T' && decoded_current_pos_[3] == 'P')
	{

		decoded_current_pos_ = strstr(decoded_current_pos_, "\r\n\r\n");
		if (decoded_current_pos_ != nullptr)
		{
			decoded_current_pos_ += 2;
		}
	}
	// get chunk size
	decoded_current_pos_ = strstr(decoded_current_pos_,"\r\n");
	if(decoded_current_pos_ != nullptr)
	{
		if(decoded_current_pos_ - recved_buffer_ < MAX_LENGTH_OF_RECEIVE_MESSAGE - 2)
		{
			decoded_current_pos_ += 2;
			char tmp[10] = {0};
			char* pEnd;
			for(int i = 0; i < 10; i++)
			{
				if(decoded_current_pos_[0] == '\r') break;
				tmp[i] = decoded_current_pos_[0];
				decoded_current_pos_++;
			}
			chunk_size = strtol(tmp,&pEnd,10);
			decoded_current_pos_ = strstr(decoded_current_pos_,"\r\n");
			decoded_current_pos_ += 2;
			memcpy (decoded_buffer_,decoded_current_pos_,chunk_size);

		}
	}
	return chunk_size;
}

bool FlvHttp::get_received_msg(int msg_size,char* payload)
{
	// decode the chunked data and copy the data to decoded_buffer_

	int temp = 0;
	do
	{
		temp = decode_one_chunked();
		
		if(temp == 0)
		{
			cout<<"fail to decode one chunk "<<endl;
			return false;
		}

		memcpy(payload,decoded_buffer_,msg_size <= temp ? msg_size:temp);
		msg_size -= temp;
	}while(msg_size > 0);
	
	return true;
	/*
	char*  whole_message = nullptr;
	whole_message = client_socket_.recv_msg(msg_size);
	if(msg_size == 0)
	{
		cout<<"the http response is null"<<endl;
		return nullptr;
	}
	return parse_received_msg(whole_message, msg_size);
	*/
}
