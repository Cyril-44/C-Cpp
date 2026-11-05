// ArbiterPlus 独立服务器工程 —— c-s/common/log.cpp
#include "log.h"
#include <cstdarg>
#include <ctime>
#include <cstring>

FILE*    APLog::m_fp    = NULL;
LogLevel APLog::m_level = Debug;

void APLog::init(const char* file)
{
    if (file == NULL || *file == '\0') {
        close();
        return;
    }
    m_fp = fopen(file, "a");
    if (m_fp == NULL)
        m_fp = stdout;
}

void APLog::close()
{
    if (m_fp != NULL && m_fp != stdout) fclose(m_fp);
    m_fp = NULL;
}

void APLog::printf(LogLevel level, const char* format, ...)
{
    if ((int)m_level > (int)level || m_fp == NULL)
        return;

    va_list ap;
    va_start(ap, format);

    time_t t = time(NULL);
    struct tm* local = localtime(&t);
    switch (level) {
    case Debug:
    case Normal:   fputc('N', m_fp); break;
    case Alarm:    fputc('W', m_fp); break;
    case System:
    case Critical: fputc('E', m_fp); break;
    default:       fputc('N', m_fp); break;
    }
    char stamp[32];
    if (strftime(stamp, sizeof(stamp), "%m-%d %H:%M:%S ", local))
        fputs(stamp, m_fp);
    vfprintf(m_fp, format, ap);
    fputc('\n', m_fp);
    fflush(m_fp);
    va_end(ap);
}
