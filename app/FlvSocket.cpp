//
//Created by Hank on 2016/3/4.
//

#include "FlvSocket.h"
using namespace FlvSocket;

FlvBaseSocket::FlvBaseSocket(){

}

FlvBaseSocket::~FlvBaseSocket()
{

}


FlvTCPSocket::FlvTCPSocket():socket_(-1),
	socket_time_out_(DEFAULT_SOCKET_TIME_OUT),
	message_(nullptr)

{
	message_ = new char[MAX_LENGTH_OF_RECEIVE_MESSAGE];
}

FlvTCPSocket::~FlvTCPSocket()
{
	if(socket_ != -1)
		close(socket_);
	if(message_ != nullptr) delete[] message_;
}

bool FlvTCPSocket::initialize(std::string TargetIpAddr, int32_t TargetPort)
{
	socket_ = socket(AF_INET,SOCK_STREAM,0);
	
	if(socket_ == -1) 
	{
		std::cout<<"fail to create local socket"<<std::endl;
		return false;
	}

	socket_addr_.sin_family = AF_INET;
	socket_addr_.sin_port = htons(TargetPort);
	socket_addr_.sin_addr.s_addr = inet_addr(TargetIpAddr.c_str());

	setsockopt(socket_,SOL_SOCKET,SO_RCVTIMEO,(char*)&socket_time_out_,sizeof(int32_t));

	// connect to server
	if (connect(socket_,(const sockaddr*)&socket_addr_,sizeof(socket_addr_)) != 0)
	{
		std::cout<<"fail to connect to the server"<<std::endl;
		return false;
	}

	std::cout<<"success to create socket and connect to server"<<std::endl;
	return true;
}

int32_t FlvTCPSocket::send_msg(std::string msg, int32_t msg_size)
{
	int32_t ret = 0;
	if(socket_  == -1)
	{
		std::cout<<"the socket has not been created"<<std::endl;
		return ret;
	}
	//const char* test = "GET /live/livestream.flv HTTP/1.1\r\nHost: 192.168.9.237:8080\r\n\r\n";
	ret = send(socket_,msg.c_str(),msg_size,0);
	//ret = send(socket_,test,(size_t)strlen(test),0);
	std::cout<<"the length of message sended is "<<ret<<std::endl;
	return ret;
}

char* FlvTCPSocket::recv_msg(int32_t& msg_size)
{
	if(socket_  == -1)
	{
		std::cout<<"the socket has not been created"<<std::endl;
		return nullptr;
	}
	msg_size = recv(socket_,message_,MAX_LENGTH_OF_RECEIVE_MESSAGE,0);
	std::cout<<"the length of message received is "<<msg_size<<std::endl;
	return message_;
}

FlvUDPSocket::FlvUDPSocket()
{

}

FlvUDPSocket::~FlvUDPSocket()
{

}

bool FlvUDPSocket::initialize(std::string TargetIpAddr,int32_t TargetPort)
{
	return true;
}

int32_t FlvUDPSocket::send_msg(std::string msg, int32_t msg_size)
{
	return 0;
}

char* FlvUDPSocket::recv_msg(int32_t& msg_szie)
{

}
