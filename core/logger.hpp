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
}  // namespace ye

#define YE_ENGINE_ERROR(...)                                           \
  do {                                                                 \
    ::ye::Logger::GetSingleton().GetEngineLogger().error(__VA_ARGS__); \
    ::ye::Logger::GetSingleton().GetEngineLogger().dump_backtrace();   \
  } while (0)

#define YE_ENGINE_WARN(...)                                           \
  do {                                                                \
    ::ye::Logger::GetSingleton().GetEngineLogger().warn(__VA_ARGS__); \
    ::ye::Logger::GetSingleton().GetEngineLogger().dump_backtrace();  \
  } while (0)

#define YE_ENGINE_TRACE(...) ::ye::Logger::GetSingleton().GetEngineLogger().trace(__VA_ARGS__)
#define YE_ENGINE_DEBUG(...) ::ye::Logger::GetSingleton().GetEngineLogger().debug(__VA_ARGS__)

#define YE_CLIENT_ERROR(...)                                           \
  do {                                                                 \
    ::ye::Logger::GetSingleton().GetClientLogger().error(__VA_ARGS__); \
    ::ye::Logger::GetSingleton().GetClientLogger().dump_backtrace();   \
  } while (0)

#define YE_CLIENT_WARN(...)                                           \
  do {                                                                \
    ::ye::Logger::GetSingleton().GetClientLogger().warn(__VA_ARGS__); \
    ::ye::Logger::GetSingleton().GetClientLogger().dump_backtrace();  \
  } while (0)

#define YE_CLIENT_TRACE(...) ::ye::Logger::GetSingleton().GetClientLogger().trace(__VA_ARGS__)
#define YE_CLIENT_DEBUG(...) ::ye::Logger::GetSingleton().GetClientLogger().debug(__VA_ARGS__)

#if not defined(YE_SUPPRESS_ENGINE_INFO) and not defined(YE_RELEASE)
#define YE_ENGINE_INFO(...) ::ye::Logger::GetSingleton().GetEngineLogger().info(__VA_ARGS__)
#else
#define YE_ENGINE_INFO(...)
#endif

#ifndef YE_RELEASE
#define YE_CLIENT_INFO(...) ::ye::Logger::GetSingleton().GetClientLogger().info(__VA_ARGS__)
#else
#define YE_CLIENT_INFO(...)
#endif
