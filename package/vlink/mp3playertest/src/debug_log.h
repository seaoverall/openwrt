#ifndef __DEBUG_LOG_H
#define __DEBUG_LOG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <sys/time.h>

#if 1
#define LOGDEBUG
#endif

#define LOG2FILE

#define LOG_TIME_BUFFER_SIZE    (64)        //log header size
#define LOG_BUFFER_SIZE         (1024)      //log buffer size
#define LOGFILENAME             "/tmp/lteinfo.txt"


#ifndef LOGDEBUG
void inline log_debug(const char *fmt, ...)
{
}
#else
extern void inline log_debug(const char *fmt, ...);
void inline log_debug(const char *fmt, ...)
{
    time_t timep;
    struct timeval tv;
    struct tm *tm = NULL;
    va_list ap; 
    char   header[LOG_TIME_BUFFER_SIZE];
    FILE   *pFile = NULL;

    gettimeofday(&tv, NULL);
    timep = tv.tv_sec;
    tm = localtime(&timep);
    sprintf(header, "[%04d%02d%02d-%02d:%02d:%02d.%06d]", 
            (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday,
            tm->tm_hour, tm->tm_min, tm->tm_sec, (int)(tv.tv_usec));

    va_start(ap, fmt); 
#ifdef LOG2FILE
    pFile = fopen(LOGFILENAME, "a+");
    if (NULL == pFile)
    {
        goto failed;
    }

    fprintf(pFile, "%s", header);
    vfprintf(pFile, fmt, ap);
    fflush(pFile);
    fclose(pFile);
    pFile = NULL;
#else
    fprintf(stdout, "%s", header); 
    vfprintf(stdout, fmt, ap); 
    fflush(stdout); 
#endif //LOG2FILE

failed:
    va_end(ap); 
}
#endif //LOGDEBUG

#endif //__DEBUG_LOG_H
