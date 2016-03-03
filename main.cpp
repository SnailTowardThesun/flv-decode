#include "kernel/FlvKernel.h"
#include "FlvDecode.h"
int main(int argc , char* argv[])
{
    std::cout << VERSION<< std::endl;
    FlvDecode decode;
    if(argc > 1)
    {
        decode.decode_flv_file(argv[1]);
    }
    else decode.decode_flv_file("test.flv");
    return 0;
}
