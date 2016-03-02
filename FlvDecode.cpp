//
// Created by Hank on 2016/3/2.
//

#include "FlvDecode.h"

FlvDecode::FlvDecode()
{

}

FlvDecode::~FlvDecode()
{

}

void FlvDecode::decode_flv_file(std::string filename)
{
    if(filename.empty())
    {
        std::cout<<"the file name is empty"<<std::endl;
        return;
    }
    std::cout<<"the file to be decoded has a name "<<filename<<std::endl;
}
