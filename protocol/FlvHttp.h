//
//Created by Hank 2016/3/4
//

#ifndef FLV_DECODE_FLVHTTP_H
#define FLV_DECODE_FLVHTTP_H

#include "../app/FlvSocket.h"
#include "../kernel/FlvCommonHeaderFile.h"

using namespace FlvSocket;
using namespace std;

class FlvHttp
{
public:
	FlvHttp();
	virtual ~FlvHttp();
	bool initialize(string TargetAddr, int32_t TargetPort);
private:
	FlvTCPSocket client_socket_;
	string host_ip_;
	string host_port_;
	char* decoded_buffer_;
	char* recved_buffer_;
	char* decoded_current_pos_;

	int32_t data_left_;
protected:
	char* parse_received_msg(char* msg_to_parse, int32_t& msg_size);
	int32_t parse_url(string url_to_parse, string& targetAddr, string& targetPort, string& requestContent);
	int32_t decode_one_chunked();
public:
	bool send_GET_request(string msg);
	bool send_POST_request(string msg);
	bool get_received_msg(int msg_size,char* payload);
	

};


#endif
