#include "kernel/FlvKernel.h"
#include "FlvDecoder.h"

typedef enum _enInputType
{
    INPUT_NONE = 0,
    INPUT_FILE ,
    INPUT_HTTP_FLV_STREAM,
    INPUT_HTTP_HLS_STREAM,
    INPUT_RTMP_STREAM,
    INPUT_RTSP_STREAM,
}InputType;

InputType get_input_type(std::string input)
{
    if(input.empty())
    {
        FlvLog::getInstance()->trace("error","main.cpp","the input string is empty");
        return INPUT_NONE;
    }
    std::size_t the_protocol_header_size = input.find_first_of(":");
    // file
    if(the_protocol_header_size == std::string::npos)
        return INPUT_FILE;
    // http
    if(input.compare(0,4,"http") == 0)
    {
        if(input.compare(input.size() - 3,input.size(),"flv") == 0)
            return INPUT_HTTP_FLV_STREAM;
        if(input.compare(input.size() - 4, input.size(),"m3u8") == 0)
            return INPUT_HTTP_HLS_STREAM;
    }
    // rtmp 
    if(input.compare(0,4,"rtmp") == 0)
        return INPUT_RTMP_STREAM;
    // rtsp
    if(input.compare(0,4,"rtsp") == 0)
        return INPUT_RTSP_STREAM;
    return INPUT_NONE;
}

int main(int argc , char* argv[])
{
    std::cout << VERSION<< std::endl;
    if(argc < 2)
    {
        FlvLog::getInstance()->trace("error","main.cpp","no file or stram to decode");
        return -1;
    }
    FlvDecode decode;
    // parse the input type
    std::string input = argv[1];
    switch(get_input_type(input))
    {
    case INPUT_FILE:
        decode.decode_flv_file(input);
        break;
    case INPUT_HTTP_FLV_STREAM:
        decode.decode_flv_from_http(input);
        break;
    case INPUT_HTTP_HLS_STREAM:
        break;
    case INPUT_RTMP_STREAM:
        break;
    case INPUT_RTSP_STREAM:
        break;
    default:
        break;
    }
    return 0;
}
