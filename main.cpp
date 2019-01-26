#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <android/log.h>
#include <nfa_dm_int.h>
#include "xhook.h"

#define LOG_TAG "NFC SETUID Module ==> "

#define LOG(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)

#define MyHook(FUN) xhook_register(".*libnfc-nci\\.so$", (#FUN), \
                            reinterpret_cast<void *>(my_##FUN), reinterpret_cast<void **>(&sys_##FUN));


tNFC_STATUS (*sys_NFC_Deactivate)(tNFC_DEACT_TYPE deactivate_type);

bool (*sys_nfa_dm_set_config)(tNFA_DM_MSG *p_data);

tNFC_STATUS my_NFC_Deactivate(tNFC_DEACT_TYPE deactivate_type) {
    LOG("my_NFC_Deactivate");
    return sys_NFC_Deactivate(deactivate_type);
}

bool my_nfa_dm_set_config(tNFA_DM_MSG *p_data) {
    LOG("my_nfa_dm_set_config");
    return sys_nfa_dm_set_config(p_data);
}


void setNfcId(void) {
    LOG("Set NFCID Start");
    sleep(1);
    my_NFC_Deactivate(NFC_DEACTIVATE_TYPE_IDLE);
    // note: do not sleep here!!!!!!
    // sleep(5);
    uint8_t buffer[32] = {0};
    uint8_t *p = buffer;
    UINT8_TO_STREAM(p, 0x4d);
    UINT8_TO_STREAM(p, 0xf7);
    UINT8_TO_STREAM(p, 0xf4);
    UINT8_TO_STREAM(p, 0x40);
    tNFA_DM_MSG data;
    data.setconfig = tNFA_DM_API_SET_CONFIG{{}, NFC_PMID_LA_NFCID1, (uint8_t) (p - buffer), buffer};
    my_nfa_dm_set_config(&data);
    my_NFC_Deactivate(NFC_DEACTIVATE_TYPE_DISCOVERY);
    LOG("Set NFCID Complete");
}


void __attribute__ ((constructor)) hook_entry() {
    LOG("constructor, hia hia hia~");

    MyHook(NFC_Deactivate);
    MyHook(nfa_dm_set_config);
    xhook_refresh(0);

    setNfcId();

    LOG("constructor complete");
    return;
}
