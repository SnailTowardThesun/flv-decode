//
//Created by Hank 2016/3/4
//

#include "FlvHttp.h"
#include <string.h>

FlvHttp::FlvHttp():decoded_buffer_(nullptr),
	recved_buffer_(nullptr),
	decoded_current_pos_(nullptr),
	data_left_(0),
	data_coded_type_(CHUNKED)
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

	// we should get response message here 
	// set the type of data(chunked or gzip_)
	int32_t data_received_size = 0;
	char* pbuffer = recved_buffer_ = client_socket_.recv_msg(data_received_size);
	pbuffer = strstr(recved_buffer_,"\r\n\r\n");
	if(pbuffer != nullptr)
	{
		pbuffer += 2;
		int size = (int)(data_received_size - (pbuffer - recved_buffer_));
		recved_buffer_ = pbuffer;
		decode_one_chunked(size);
		return true;
	}

	return false;
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

int32_t FlvHttp::decode_one_chunked(int size = 0)
{	
	// the message copy always start at the begining of decode_buffer_
	memset(decoded_buffer_,0,MAX_LENGTH_OF_RECEIVE_MESSAGE);
	data_left_ = 0;

	int32_t data_left_to_decode = 0;
	if(size != 0)
	{
		data_left_to_decode = size;
	}
	else
	{
		recved_buffer_ = client_socket_.recv_msg(data_left_to_decode);
	}
	// decode the received message into decoded_buffer_	
	char* pdecoded = decoded_buffer_;
	char* pChunk = recved_buffer_;
	while(data_left_to_decode > 0)
	{
		if(previous_chunked_data_size_ >0)
		{
			int data_size_to_copy = \
				previous_chunked_data_size_ < data_left_to_decode?previous_chunked_data_size_:data_left_to_decode;
			
			memcpy(pdecoded, recved_buffer_,data_size_to_copy);
			data_left_ += data_size_to_copy;
			
			pdecoded += data_size_to_copy;
			data_left_to_decode -= data_size_to_copy;
			previous_chunked_data_size_ -= data_size_to_copy;
		}
		else	
		{
			pChunk = strstr(pChunk, "\r\n");
			if(pChunk != nullptr)
			{
				pChunk += 2;
				data_left_to_decode -= pChunk - recved_buffer_;
				
				// get chunk size
				char tmp[10] = {0};
				char* pEnd;
				for(int i = 0; i < 10; i++)
				{
					if(pChunk[0] == '\r') break;
					tmp[i] = pChunk[0];
					pChunk++;
					data_left_to_decode -= 1;
				}
				int chunk_size = strtol(tmp,&pEnd,16);
				cout<<endl;
				cout<<"chunk size is "<<chunk_size<<endl;
				cout<<endl;
				pChunk += 2;
				data_left_to_decode -= 2;
				if(data_left_to_decode < chunk_size)
				{
					memcpy(pdecoded+data_left_, pChunk, data_left_to_decode);
					data_left_ += data_left_to_decode;
					previous_chunked_data_size_ = chunk_size - data_left_to_decode;
					data_left_to_decode = 0;
				}
				else
				{
					memcpy(pdecoded+data_left_,pChunk, chunk_size);
					data_left_ += chunk_size;
					data_left_to_decode -= chunk_size;
					pChunk += chunk_size;
				}
			}
			else
			{
				break;
			}
		}
	}
	data_total_ = data_left_;
	return true;
}

bool FlvHttp::get_received_msg(int msg_size,char* payload)
{
	// decode the chunked data and copy the data to decoded_buffer_	
	int tmp = 0;
	while(msg_size > 0)
	{
		if(msg_size <= data_left_)
		{
			memcpy(payload + tmp,&decoded_buffer_[data_total_ - data_left_],msg_size);
			data_left_ -= msg_size;
			msg_size = 0;
		}
		else
		{	
			memcpy(payload + tmp,&decoded_buffer_[data_total_ - data_left_ ],data_left_);
			msg_size -= data_left_;
			tmp += data_left_;
			decode_one_chunked();
		}
	}

	return true;	
}
