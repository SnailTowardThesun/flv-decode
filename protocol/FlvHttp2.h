//
//Created by Hank 2016/3/8
//
#ifndef FLV_DECODE_FLVHTTP2_HEAD
#define FLV_DECODE_FLVHTTP2_HEAD
#include "../kernel/FlvCommonHeaderFile.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <csignal>
static const int  MAX_LENGTH_OF_RECEIVE_MESSAGE = 4096;
using namespace std;

class FlvHttp2
{
public:
    FlvHttp2();
    virtual ~FlvHttp2();
private:
    int socket_;
    int32_t  socket_time_out_;
    struct sockaddr_in socket_addr_;
    FILE* file_p_;
    string host_ip_;
    string host_port_;
    char* message_;
    int chunk_size_left_;
protected:
    int get_chunk_size();
public:
    bool initialize(string url, int port);
    bool send_GET_request(string msg);
    bool get_received_msg(int message_size, char* message);
};

#endif
