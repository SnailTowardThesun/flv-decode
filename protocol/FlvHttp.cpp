//
//Created by Hank 2016/3/4
//

#include "FlvHttp.h"

FlvHttp::FlvHttp()
{
	
}

FlvHttp::~FlvHttp()
{

}

bool FlvHttp::initialize(string TargetAddr, int32_t TargetPort)
{
	if(!client_socket_.initialize(TargetAddr,TargetPort))
	{
		cout<<"fail to initialize client_socket_"<<endl;
		return false;
	}

	return true;
}

bool FlvHttp::send_GET_request(string msg)
{
	if(msg.empty()) 
	{
		cout<<"the request message to be sended by GET is empty"<<endl;
		return false;
	}
	string get_request = "Get /live/livestream.flv /HTTP/1.1\\n";
	get_request += "Expect: 100-continue\\n\\r\\n";

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

int32_t FlvHttp::parse_received_msg(string msg_to_parse, string& msg_result)
{
	int32_t ret = 0;

	if(msg_to_parse.empty())
	{
		return ret;
	}

	msg_result = msg_to_parse[1];
	return ret;
}

bool FlvHttp::get_received_msg(string& msg)
{
	string whole_message;
	int32_t whole_message_size = 0;
	client_socket_.recv_msg(whole_message, whole_message_size);
	if(whole_message_size == 0)
	{
		cout<<"the http response is null"<<endl;
		return false;
	}
	parse_received_msg(whole_message,msg);
	return true;
}
