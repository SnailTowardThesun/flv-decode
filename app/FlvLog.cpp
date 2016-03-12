//
// Created by Hank on 2016/3/2.
//

#include "FlvLog.h"
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>


FlvLog* FlvLog::instance_ = nullptr;

FlvLog* FlvLog::getInstance()
{
	if(instance_ == nullptr)
		instance_ = new FlvLog();
	return instance_;
}

void FlvLog::releaseInstance()
{
	if(instance_ != nullptr) delete instance_;
}

FlvLog::FlvLog():file_name_(DEFAULT_LOG_FILE),
	log_data_(nullptr),
	flog_file_(-1)
{
	log_data_ = new char[LOG_MAX_SIZE];
	if(flog_file_ > 0) ::close(flog_file_);
}

FlvLog::~FlvLog()
{
	if(log_data_ != nullptr) delete[] log_data_;
}

void FlvLog::set_log_file(const std::string fileName)
{
	file_name_ = fileName;
}

bool FlvLog::generate_header(const char* tag, int context_id, const char* level_name, int& header_size)
{

	// clock time
    timeval tv;
    if (gettimeofday(&tv, NULL) == -1) 
	{
        return false;
    }
    
    // to calendar time
    struct tm* tm;
    
	if ((tm = localtime(&tv.tv_sec)) == NULL) 
	{
		return false;
	}
    
    // write log header
    int log_header_size = -1;
     
    if (tag) 
	{
        log_header_size = snprintf(log_data_, LOG_MAX_SIZE, 
                "[%d-%02d-%02d %02d:%02d:%02d.%03d][%s][%s][%d][%d] ", 
                1900 + tm->tm_year, 1 + tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (int)(tv.tv_usec / 1000), 
                level_name, tag, getpid(), context_id);
    } 
	else 
	{
        log_header_size = snprintf(log_data_, LOG_MAX_SIZE, 
                "[%d-%02d-%02d %02d:%02d:%02d.%03d][%s][%d][%d] ", 
                1900 + tm->tm_year, 1 + tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (int)(tv.tv_usec / 1000), 
                level_name, getpid(), context_id);
    }

    if (log_header_size == -1) 
	{
        return false;
    }
    
    // write the header size.
    header_size = LOG_MAX_SIZE - 1 < log_header_size ? LOG_MAX_SIZE-1 : log_header_size;
    

	return true;
}

int32_t FlvLog::log(const char* level_name, const char* tag, const char* fmt,...)
{
	int header_size = 0,size = 0;
	if(!generate_header(tag,0,level_name,header_size))
	{
		return RESULT_ERROR;
	}

	va_list ap;
	va_start(ap,fmt);
	
	size = header_size + vsnprintf(log_data_ + header_size ,LOG_MAX_SIZE - header_size,fmt,ap);
	va_end(ap);

	size = size > LOG_MAX_SIZE - 1 - 1 ? LOG_MAX_SIZE - 1 - 1 : size;

    flog_file_ = ::open(file_name_.c_str(), O_RDWR | O_APPEND);
    
    if(flog_file_ == -1) {
        flog_file_ = ::open(file_name_.c_str(), 
            O_RDWR | O_CREAT | O_TRUNC, 
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
        );
    }
	if(flog_file_ > 0)
	{
		::write(flog_file_,log_data_,size);
		::write(flog_file_,"\n",1);
	}
	return 0;
}

void FlvLog::trace(std::string level_name, std::string tag, std::string msg)
{
	std::cout<<"["<<level_name<<"]["<<tag<<"]"<<msg<<std::endl;
}
