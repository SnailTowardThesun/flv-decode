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
	enum _enDataCoeded
	{
		CHUNKED = 0,
		GZIP,
	};
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

	int32_t data_left_,data_total_;
	int32_t previous_chunked_data_size_;
	_enDataCoeded  data_coded_type_;
protected:
	int32_t parse_url(string url_to_parse, string& targetAddr, string& targetPort, string& requestContent);
	int32_t decode_one_chunked(int size);

public:
	bool send_GET_request(string msg);
	bool send_POST_request(string msg);
	bool get_received_msg(int msg_size,char* payload);
	

};


#endif
