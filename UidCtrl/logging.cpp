//
// Created by joe on 19-4-7.
//

#include "logging.hpp"
#include <stdarg.h>
#include <android/log.h>

Log &Log::getInstance(LOG_LEVEL level) {
    static Log log(level);
    return log;
}

void Log::print(const Log::LOG_LEVEL level, const char *tag, const char *text) {
    if (level < m_log_level || level == LOG_LEVEL_NONE)
        return;

    android_LogPriority prio;
    switch (level) {
        case LOG_LEVEL_INFO:
            prio = ANDROID_LOG_VERBOSE;
            break;
        case LOG_LEVEL_DEBUG:
            prio = ANDROID_LOG_DEBUG;
            break;
        case LOG_LEVEL_ERROR:
            prio = ANDROID_LOG_ERROR;
            break;
        default:
            prio = ANDROID_LOG_UNKNOWN;
            break;
    }

    __android_log_write(prio, tag, text);
}

void Log::printf(const LOG_LEVEL level, const char *tag, const char *fmt, ...) {
    if (level < m_log_level || level == LOG_LEVEL_NONE)
        return;

    android_LogPriority prio;
    switch (level) {
        case LOG_LEVEL_INFO:
            prio = ANDROID_LOG_VERBOSE;
            break;
        case LOG_LEVEL_DEBUG:
            prio = ANDROID_LOG_DEBUG;
            break;
        case LOG_LEVEL_ERROR:
            prio = ANDROID_LOG_ERROR;
            break;
        default:
            prio = ANDROID_LOG_UNKNOWN;
            break;
    }

    va_list args;
    va_start(args, fmt);
    __android_log_vprint(prio, tag, fmt, args);
    va_end(args);
}

Log::Log(Log::LOG_LEVEL level) {
    m_log_level = level;
}

Log::LOG_LEVEL Log::getLogLevel() const {
    return m_log_level;
}

void Log::setLogLevel(const Log::LOG_LEVEL level) {
    m_log_level = level;
}

