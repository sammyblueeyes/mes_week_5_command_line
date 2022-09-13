#pragma once
#include <io_mapping.h>

#ifdef __cplusplus
 extern "C" {
#endif

static bool g_logging_enabled = true;

enum eLogSubSystem {
    LOG_SUBSYSTEM_MAIN,
};

enum eLogLevel {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
};

void Log(eLogSubSystem sys, eLogLevel level, const char* msg) {
    if (!g_logging_enabled) return;
    io_logging_write(msg, strlen(msg));
}


void LoggingInit() {
    io_logging_init();
}

void LogGlobalOn() {
    g_logging_enabled = true;
}

void LogGlobalOff() {
    g_logging_enabled = false;
}


/*
void LogWithNum(enum eLogSubSystem sys, eLogLevel level, int number) {
    if (!g_logging_enabled) return;
}

void LogOutputLevel(enum eLogSubSystem sys, enum eLoglevel level) {
    if (!g_logging_enabled) return;
}
*/

// TODO
// void LogVersion(struct sFirmwareVerions *v)


#ifdef __cplusplus
 } // extern "C"
#endif
