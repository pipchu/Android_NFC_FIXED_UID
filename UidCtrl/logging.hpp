//
// Created by joe on 19-4-7.
//

#ifndef UIDCTRL_LOG_HPP
#define UIDCTRL_LOG_HPP


class Log {
public:
    enum LOG_LEVEL {
        LOG_LEVEL_INFO,
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_NONE
    };

private:
    LOG_LEVEL m_log_level;

public:
    static Log &getInstance(LOG_LEVEL level = LOG_LEVEL_NONE);

    void printf(const LOG_LEVEL level, const char *tag, const char *fmt, ...);

    void print(const LOG_LEVEL level, const char *tag, const char *text);

    LOG_LEVEL getLogLevel() const;

    void setLogLevel(const LOG_LEVEL);

private:
    Log(LOG_LEVEL);

    Log(const Log &) {}

    ~Log() {};

    Log &operator=(const Log &) {}

};


#endif //UIDCTRL_LOG_HPP
