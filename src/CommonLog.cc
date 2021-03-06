#include  "Logger.h"
#include  <cstdarg>
#include  <ctime>
#include <sys/types.h>
#include <unistd.h>

static const  char* Path =  "/var/log/flynet/error.log";

Logger::CommonLog::CommonLog()
        : level_(Logger::DEBUG)
        , fp_(nullptr){
        if(-1 == ::system("mkdir -p /var/log/flynet/")){
            //后续改成写到系统日志里面
            printf("system error:  %s\n", strerror(errno));
            exit(-1);
        }
        if(nullptr == (fp_ =  fopen(Path,  "a+"))){
            printf("fopen error:  %s\n", strerror(errno));
            exit(-1);
        }
}

Logger::CommonLog::~CommonLog(){
        printf("~CommonLog \n");
        fclose(fp_);
}

void Logger::CommonLog::WriteLogEx(std::size_t lv, const char* fmt, SrcFile& file, ...){
    if(lv < level_)
        return ;

    buffer_.Clear();
    va_list ap;
    tm t;
    va_start(ap, fmt);
    time_t now = time(nullptr);
    localtime_r((const time_t *)&now, &t);
    int32_t line = va_arg(ap, int32_t);
    const char* func = va_arg(ap, const char* );
    //注意sprintf从右向左入栈，这种写法sprintf("....",  va_arg, va_arg)有问题
    int32_t bytes = snprintf((char*)buffer_.GetBuffer(), buffer_.Size(), "[%02d:%02d:%02d][%05d][%s:%d][%s]",
               t.tm_hour, t.tm_min, t.tm_sec, getpid(), file.Name(), line , func);
    bytes += vsnprintf((char*)buffer_.GetBuffer() + bytes, buffer_.Size(), fmt, ap);
    va_end(ap);
    int cnt = fwrite(buffer_.GetBuffer(),  1,  bytes,  fp_);
    fflush(fp_);
}

bool Logger::CommonLog::SetLevelEx(int32_t level){
       if(level >= Logger::DEBUG  && level <= Logger::FATAL){
           level_ = level;
           return true;
       } 
        return false;
}
