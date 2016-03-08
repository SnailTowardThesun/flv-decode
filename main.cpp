#include "kernel/FlvKernel.h"
#include "FlvDecode.h"
int main(int argc , char* argv[])
{
    std::cout << VERSION<< std::endl;
    FlvDecode decode;
	/*
    if(argc > 1)
    {
        decode.decode_flv_file(argv[1]);
    }
    else decode.decode_flv_file("test.flv");
	*/
	decode.decode_flv_from_http("http://192.168.9.237:8080/live/livestream.flv");
    return 0;
}
