#pragma once

#include "core/assertion.hpp"
#include "core/error_enum.hpp"
#define SPDLOG_CLOCK_COARSE
#define SPDLOG_DISABLE_DEFAULT_LOGGER
#include <memory>
#include <spdlog/logger.h>
#include <filesystem>

namespace ye {
class Logger {
 private:
  static const std::filesystem::path FILE_LOG_PATH;
  static const std::chrono::system_clock::time_point LAUNCH_TIMESTAMP;

  static Logger* s_singleton;
  Logger() noexcept = default;

 private:
  static std::shared_ptr<spdlog::logger> CreateLogger(const char* name, bool enable_file, bool enable_console);
  std::shared_ptr<spdlog::logger> m_engine_logger;
  std::shared_ptr<spdlog::logger> m_client_logger;

 public:
  eError CreateClientLogger(const char* name, bool file_log, bool console_log);

  spdlog::logger& GetEngineLogger() const noexcept { return *m_engine_logger; }
  spdlog::logger& GetClientLogger() const noexcept {
    YE_ASSERT(m_client_logger != nullptr, "Client logger is not initialized");
    return *m_client_logger;
  }

 public:
  static bool Exist() { return s_singleton != nullptr; }
  static Logger& GetSingleton() {
    YE_ASSERT(s_singleton != nullptr, "Logger is not initialized");
    return *s_singleton;
  }

#ifdef YE_DEBUG
  static eError Create(bool file_log = false, bool console_log = true);
#elif defined(YE_RELEASE)
  static eError Create(bool file_log = true, bool console_log = false);
#else
  static eError Create(bool file_log = true, bool console_log = true);
#endif

  ~Logger() = default;
  void Shutdown();
};

#define YE_ENGINE_ERROR(...) Logger::GetSingleton().GetEngineLogger().error(__VA_ARGS__)
#define YE_ENGINE_WARN(...) Logger::GetSingleton().GetEngineLogger().warn(__VA_ARGS__)
#define YE_ENGINE_TRACE(...) Logger::GetSingleton().GetEngineLogger().trace(__VA_ARGS__)

#ifdef YE_DEBUG
#define YE_ENGINE_INFO(...) Logger::GetSingleton().GetEngineLogger().info(__VA_ARGS__)
#else
#define YE_ENGINE_INFO(...)
#endif

#ifdef YE_LOGGER_CLIENT_MACROS
#define YE_CLIENT_ERROR(...) Logger::GetSingleton().GetClientLogger().error(__VA_ARGS__)
#define YE_CLIENT_WARN(...) Logger::GetSingleton().GetClientLogger().warn(__VA_ARGS__)
#define YE_CLIENT_TRACE(...) Logger::GetSingleton().GetClientLogger().trace(__VA_ARGS__)

#ifdef YE_DEBUG
#define YE_CLIENT_INFO(...) Logger::GetSingleton().GetClientLogger().info(__VA_ARGS__)
#else
#define YE_CLIENT_INFO(...)
#endif
#endif

}  // namespace ye
