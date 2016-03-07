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
protected:
	int32_t parse_received_msg(string msg_to_parse, string& msg_result);
public:
	bool send_GET_request(string msg);
	bool send_POST_request(string msg);

	bool get_received_msg(string& msg);
	

};


#endif
