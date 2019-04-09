//
// Created by joe on 19-4-7.
//

#ifndef UIDCTRL_HOOK_HPP
#define UIDCTRL_HOOK_HPP


#include "logging.hpp"
#include <pthread.h>
#include <nfa_dm_int.h>

class Hook {
private:
    bool m_hook;
    bool m_cpp;
    pthread_mutex_t m_hook_mutex;
    Log &log = Log::getInstance();

public:
    static Hook &getInstance(bool cpp = true);

    void hook();

// third-party library xHook does not support unhook currently
//    void dehook();

    bool setNfcId(uint8_t *uid);

    bool isHooked();

    bool isCpp() const;

private:
    Hook(bool cpp) : m_hook(false), m_cpp(cpp) {
        pthread_mutex_init(&m_hook_mutex, nullptr);
    }

    ~Hook() {
        pthread_mutex_destroy(&m_hook_mutex);
    };

    Hook(const Hook &) {}

    Hook &operator=(const Hook &) {}
};


#endif //UIDCTRL_HOOK_HPP
