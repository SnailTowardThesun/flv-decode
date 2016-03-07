//
//Created by Hank on 2016/3/4
//

#ifndef FLV_DECODE_FLVSOCKET_H
#define FLV_DECODE_FLVSOCKET_H

#include "../kernel/FlvCommonHeaderFile.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <csignal>

static const int32_t DEFAULT_SOCKET_TIME_OUT = 2000;
static const int32_t MAX_LENGTH_OF_RECEIVE_MESSAGE = 4096;
namespace FlvSocket
{

	class FlvBaseSocket // base socket class
	{
	public:
		FlvBaseSocket();
		virtual ~FlvBaseSocket();
	public:
		virtual bool  initialize(std::string TargetIpAddr,int32_t localPort) = 0;
		virtual int32_t send_msg(std::string msg, int32_t msg_size) = 0;
		virtual char* recv_msg( int32_t& msg_size) = 0;
	};

	class FlvTCPSocket:FlvBaseSocket // TCP socket
	{
	public:
		FlvTCPSocket();
		virtual ~FlvTCPSocket();
	private:
		int socket_;
		int32_t  socket_time_out_;
		struct sockaddr_in socket_addr_;
		char* message_;

	public:
		bool initialize(std::string TargetIpAddr,int32_t TargetPort);
		int32_t send_msg(std::string msg, int32_t msg_size);
		char* recv_msg( int32_t& msg_size);
	};

	class FlvUDPSocket:FlvBaseSocket
	{
	public:
		FlvUDPSocket();
		virtual ~FlvUDPSocket();
	public:
		bool initialize(std::string TargetIpAddr,int32_t TargetPort);
		int32_t send_msg(std::string msg, int32_t msg_size);
		char* recv_msg( int32_t& msg_size);

	};
};

#endif
