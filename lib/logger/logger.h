#pragma once
#include <io_mapping.h>
#include <memory>

enum eLogLevel {
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARN,
  LOG_LEVEL_ERROR,
};

class LoggingBackend {
public:
  virtual void Init() = 0;
  virtual void Write(const char *buffer, size_t len) = 0;
};

class Logger {
private:
  const char *m_subsystem;
  std::unique_ptr<LoggingBackend> m_backend;

public:
  Logger(const char *subsystem, std::unique_ptr<LoggingBackend> backend);
  Logger(const Logger &) = delete;

  void Log(eLogLevel level, const char *msg);

  void LogGlobalOn();

  void LogGlobalOff();

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
};