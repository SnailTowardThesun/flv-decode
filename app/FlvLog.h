//
// Created by Hank on 2016/3/2.
//

#ifndef FLV_DECODE_FLVLOG_H
#define FLV_DECODE_FLVLOG_H

#include "../kernel/FlvCommonHeaderFile.h"

static const int32_t LOG_MAX_SIZE = 4096;
static const char* DEFAULT_LOG_FILE = "flv-decode.log";

class FlvLog 
{
public:
	FlvLog();
	virtual ~FlvLog();

	// static part
private:
	static FlvLog* instance_;
public:
	static FlvLog* getInstance();
	static void releaseInstance();
	//
private:
	std::string file_name_;
	char* log_data_;
	int flog_file_;
protected:
	bool generate_header(const char* tag, int context_id,const char* level_name, int& header_size);
public:
	void set_log_file(const std::string logFile);
	int32_t log(const char* level_name, const char* tag, const char* fmt,...);
	void trace(std::string level_name, std::string tag, std::string msg);

};


#endif //FLV_DECODE_FLVLOG_H
