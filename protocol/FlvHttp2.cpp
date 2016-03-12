//
//Created by Hank 2016/3/8
//
#include "FlvHttp2.h"
#include <stdio.h>
#include "../app/FlvSocket.h"

FlvHttp2::FlvHttp2():socket_(-1),
	socket_time_out_(2000),
	message_(nullptr),
	chunk_size_left_(0)
{
	message_ = new char[MAX_LENGTH_OF_RECEIVE_MESSAGE];
	memset(message_,0,MAX_LENGTH_OF_RECEIVE_MESSAGE);
}

FlvHttp2::~FlvHttp2()
{
	if(file_p_ != nullptr)
		fclose(file_p_);
	if(message_ != nullptr) delete[] message_;
}

bool FlvHttp2::initialize(string url, int port)
{
	if(url.empty() || port < 1) return false;
	socket_ = socket(AF_INET,SOCK_STREAM,0);
	
	if(socket_ == -1) 
	{
		std::cout<<"fail to create local socket"<<std::endl;
		return false;
	}

	socket_addr_.sin_family = AF_INET;
	socket_addr_.sin_port = htons(port);
	socket_addr_.sin_addr.s_addr = inet_addr(url.c_str());

	setsockopt(socket_,SOL_SOCKET,SO_RCVTIMEO,(char*)&socket_time_out_,sizeof(int32_t));

	// connect to server
	if (connect(socket_,(const sockaddr*)&socket_addr_,sizeof(socket_addr_)) != 0)
	{
		std::cout<<"fail to connect to the server"<<std::endl;
		return false;
	}

	host_ip_ = url;
	host_port_ = to_string(port);

	std::cout<<"success to create socket and connect to server"<<std::endl;
	file_p_ = fdopen(socket_,"rw+");
	if( file_p_ == nullptr)
	{
		cout<<"fail to create the file"<<endl;
		return false;
	}
	return true;
}

bool FlvHttp2::send_GET_request(string msg)
{
	if(msg.empty())
	{
		cout<<"the GET request's message is empty"<<endl;
		return false;
	}
	if(file_p_ == nullptr) return false;

	string request = "GET " + msg + " HTTP/1.1\r\n";
	request += "Host: " + host_ip_ + ":" + host_port_ +"\r\n\r\n";
	fwrite(request.c_str(),1,request.size(),file_p_);
	// deal the responses
	while(message_[0] != '\r' && message_[1] != '\n')
	{
		if(fgets(message_,MAX_LENGTH_OF_RECEIVE_MESSAGE,file_p_) != nullptr)
		{
			cout<<message_<<endl;
		}
	}
	return true;
}

int FlvHttp2::get_chunk_size()
{
	if (file_p_ == nullptr) return 0;
	
	int ret = 0;
	char tmp[10] = {0};
	char* pEnd;
	if (fgets(message_,32,file_p_) == nullptr)
	{
		if(feof(file_p_))
		{
			cout<<"get end of file"<<endl;
			return -1;
		}
		return -1;
	}
	for(int i = 0; i < 10; i++)
	{
		if(message_[i] == '\r') break;
		tmp[i] = message_[i];
	}
	ret = strtol(tmp,&pEnd,16);
	cout<<"chunk's size is "<<ret<<endl;
	return ret;
}

bool FlvHttp2::get_received_msg(int message_size, char* message)
{
	if(message_size < 1 || message == nullptr || file_p_ == nullptr) return false;
	cout<<"the chunk_size_left is "<<chunk_size_left_<<endl;
	int msg_required = message_size;
	while(msg_required > 0)
	{
		if(chunk_size_left_ == -1)
		{
			return false;
		}
		if(chunk_size_left_ == 0)
		{
			chunk_size_left_ = get_chunk_size();
		}
		if(msg_required < chunk_size_left_)
		{
			cout<<"get message! the size of msg_required is "<<msg_required<<endl;
			fread(message+ (message_size - msg_required),1,msg_required,file_p_);
			chunk_size_left_ -= msg_required;
			msg_required = 0;
		}
		else
		{
			cout<<"get message! the size of chunk_size_left is "<<chunk_size_left_<<endl;
			fread(message + (message_size - msg_required),1,chunk_size_left_,file_p_);
			msg_required -= chunk_size_left_;
			chunk_size_left_ = 0;
		}
	}
	
	return true;
}
