// =====================================================================
// 重建自 ArbiterPlus (NOI2022) —— c-s/common/log.h
// 依据: DWARF + Ghidra 反编译 (aclient/bclient/selfeval)
// =====================================================================
#ifndef AP_LOG_H
#define AP_LOG_H

#include <cstdio>

enum LogLevel { Debug = 0, Normal = 1, Alarm = 2, System = 3, Critical = 4 };

class APLog {
public:
    static void init(const char* file);      // 空串/打不开 -> stdout / 关闭
    static void close();
    static void printf(LogLevel level, const char* format, ...);
private:
    static FILE* m_fp;
    static LogLevel m_level;
};

#endif
