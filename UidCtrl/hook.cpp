//
// Created by joe on 19-4-7.
//

#include "hook.hpp"
#include <unistd.h>
#include "xhook.h"

#define HOOK_MACRO(FUN) xhook_register(".*libnfc-nci\\.so$", (#FUN), \
                            reinterpret_cast<void *>(my_##FUN), \
                            reinterpret_cast<void **>(&sys_##FUN));

//#define DEHOOK_MACRO(FUN) xhook_register(".*libnfc-nci\\.so$", (#FUN), \
//                            reinterpret_cast<void *>(sys_##FUN), nullptr);

const char LOG_TAG[] = "UIDCTRL:hook ==> ";


/**************** function pointer backup for C and CPP symbol ****************/
tNFC_STATUS (*sys_NFC_Deactivate)(tNFC_DEACT_TYPE deactivate_type);

bool (*sys_nfa_dm_set_config)(tNFA_DM_MSG *p_data);

// CPP
tNFC_STATUS (*sys__Z14NFC_Deactivateh)(tNFC_DEACT_TYPE deactivate_type);

bool (*sys__Z17nfa_dm_set_configP11tNFA_DM_MSG)(tNFA_DM_MSG *p_data);
/******************************************************************************/

/********************** custom func for C and CPP symbol **********************/
static tNFC_STATUS my_NFC_Deactivate(tNFC_DEACT_TYPE deactivate_type) {
    Log &log = Log::getInstance();
    log.print(Log::LOG_LEVEL_INFO, LOG_TAG, "my_NFC_Deactivate called");
    tNFC_STATUS ret = sys_NFC_Deactivate(deactivate_type);
    if (ret != NFC_STATUS_OK)
        log.printf(Log::LOG_LEVEL_ERROR, LOG_TAG, "my_NFC_Deactivate return with %u", ret);
    return ret;
}

static bool my_nfa_dm_set_config(tNFA_DM_MSG *p_data) {
    Log &log = Log::getInstance();
    log.print(Log::LOG_LEVEL_INFO, LOG_TAG, "my_nfa_dm_set_config called");
    bool ret = sys_nfa_dm_set_config(p_data);
    if (!ret)
        log.printf(Log::LOG_LEVEL_ERROR, LOG_TAG, "my_nfa_dm_set_config return with %u", ret);
    return ret;
}

// CPP
static tNFC_STATUS my__Z14NFC_Deactivateh(tNFC_DEACT_TYPE deactivate_type) {
    Log &log = Log::getInstance();
    log.print(Log::LOG_LEVEL_INFO, LOG_TAG, "my__Z14NFC_Deactivateh called");
    tNFC_STATUS ret = sys__Z14NFC_Deactivateh(deactivate_type);
    if (ret != NFC_STATUS_OK)
        log.printf(Log::LOG_LEVEL_ERROR, LOG_TAG, "my__Z14NFC_Deactivateh return with %u", ret);
    return ret;
}

static bool my__Z17nfa_dm_set_configP11tNFA_DM_MSG(tNFA_DM_MSG *p_data) {
    Log &log = Log::getInstance();
    log.print(Log::LOG_LEVEL_INFO, LOG_TAG, \
            "my__Z17nfa_dm_set_configP11tNFA_DM_MSG called");
    bool ret = sys__Z17nfa_dm_set_configP11tNFA_DM_MSG(p_data);
    if (!ret)
        log.printf(Log::LOG_LEVEL_ERROR, LOG_TAG,
                   "my__Z17nfa_dm_set_configP11tNFA_DM_MSG return with %u", ret);
    return ret;
}

/******************************************************************************/

Hook &Hook::getInstance(bool cpp) {
    static Hook hook(cpp);
    return hook;
}

void Hook::hook() {
    pthread_mutex_lock(&m_hook_mutex);

    if (!m_hook) {
        xhook_enable_debug(log.getLogLevel() == Log::LOG_LEVEL_NONE ? 0 : 1);
        if (m_cpp) {
            // CPP
            HOOK_MACRO(_Z14NFC_Deactivateh);
            HOOK_MACRO(_Z17nfa_dm_set_configP11tNFA_DM_MSG);
        } else {
            // C
            HOOK_MACRO(NFC_Deactivate);
            HOOK_MACRO(nfa_dm_set_config);

        }
        xhook_refresh(0);
        m_hook = true;
    }

    pthread_mutex_unlock(&m_hook_mutex);
}

//void Hook::dehook() {
//    pthread_mutex_lock(&m_hook_mutex);
//
//    if (m_hook) {
//        if (m_cpp) {
//            // CPP
//            DEHOOK_MACRO(_Z14NFC_Deactivateh);
//            DEHOOK_MACRO(_Z17nfa_dm_set_configP11tNFA_DM_MSG);
//        } else {
//            // C
//            DEHOOK_MACRO(NFC_Deactivate);
//            DEHOOK_MACRO(nfa_dm_set_config);
//        }
//        xhook_refresh(0);
//        m_hook = false;
//    }
//
//    pthread_mutex_unlock(&m_hook_mutex);
//}

bool Hook::isHooked() {
    pthread_mutex_lock(&m_hook_mutex);
    bool status = m_hook;
    pthread_mutex_unlock(&m_hook_mutex);

    return status;
}

bool Hook::isCpp() const {
    return m_cpp;
}

bool Hook::setNfcId(uint8_t *uid) {
    bool status = false;
    uint8_t buffer[32] = {0};
    uint8_t *p = buffer;
    tNFA_DM_MSG data;

    UINT8_TO_STREAM(p, uid[0]);
    UINT8_TO_STREAM(p, uid[1]);
    UINT8_TO_STREAM(p, uid[2]);
    UINT8_TO_STREAM(p, uid[3]);
    data.setconfig = tNFA_DM_API_SET_CONFIG{{}, NFC_PMID_LA_NFCID1, (uint8_t) (p - buffer), buffer};

//    sleep(1);
    pthread_mutex_lock(&m_hook_mutex);
    do {
        if (m_hook) {
            if (m_cpp) {
                if (my__Z14NFC_Deactivateh(NFC_DEACTIVATE_TYPE_IDLE) != NFC_STATUS_OK)
                    break;
                if (!my__Z17nfa_dm_set_configP11tNFA_DM_MSG(&data))
                    break;
                if (my__Z14NFC_Deactivateh(NFC_DEACTIVATE_TYPE_DISCOVERY) != NFC_STATUS_OK)
                    break;
            } else {
                if (my_NFC_Deactivate(NFC_DEACTIVATE_TYPE_IDLE) != NFC_STATUS_OK)
                    break;
                if (!my_nfa_dm_set_config(&data))
                    break;
                if (my_NFC_Deactivate(NFC_DEACTIVATE_TYPE_DISCOVERY) != NFC_STATUS_OK)
                    break;
            }
            status = true;
        }
    } while (false);
    pthread_mutex_unlock(&m_hook_mutex);

    return status;
}
