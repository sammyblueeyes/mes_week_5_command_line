#include <logger.h>

static bool g_logging_enabled = true;

Logger::Logger(const char *subsystem, std::unique_ptr<LoggingBackend> backend)
    : m_subsystem(subsystem), m_backend(std::move(backend)) {
  m_backend->Init();
}
void Logger::Log(eLogLevel level, const char *msg) {
  if (!g_logging_enabled) {
    return;
  }
  m_backend->Write(msg, strlen(msg));
}

void Logger::LogGlobalOn() { g_logging_enabled = true; }

void Logger::LogGlobalOff() { g_logging_enabled = false; }