#pragma once

#define SPDLOG_CLOCK_COARSE
#define SPDLOG_LEVEL_NAMES {"TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL", "OFF"}
#define SPDLOG_DISABLE_DEFAULT_LOGGER

#include <memory>
#include <spdlog/logger.h>

namespace ye {
class Logger {
 private:
  std::shared_ptr<spdlog::logger> m_pEngine_logger;
  std::shared_ptr<spdlog::logger> m_pClient_logger;

 public:
  const spdlog::logger* GetEngineLogger() const noexcept { return m_pEngine_logger.get(); }
  const spdlog::logger* GetClientLogger() const noexcept { return m_pClient_logger.get(); }

 private:
  static Logger* s_singleton;
  Logger() noexcept = default;
  void Startup();

 public:
  static Logger* GetSingleton();
  static Logger* Create();

  ~Logger() = default;
  void Shutdown();
};
}  // namespace ye
