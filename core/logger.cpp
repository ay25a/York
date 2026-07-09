#include "logger.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>

namespace ye {
Logger* Logger::s_singleton = nullptr;

std::shared_ptr<spdlog::logger> Logger::CreateLogger(const char* name, bool enable_file, bool enable_console) {
  std::vector<spdlog::sink_ptr> sinks;

  if (enable_file) {
    auto now = std::chrono::system_clock::now();
    sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(std::format("logs/{}_{:%Y-%m-%d_%H:%M:%S}.txt", name, now), true));
  }

  if (enable_console)
    sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

  auto logger = std::make_shared<spdlog::async_logger>(name, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
  logger->set_pattern("%^[%n %l | %H:%M:%S.%e]: %v%$");

#ifdef YE_DEBUG
  logger->disable_backtrace();
  logger->set_level(spdlog::level::trace);
  logger->flush_on(spdlog::level::warn);
#else
  logger->enable_backtrace(32);
  logger->flush_on(spdlog::level::err);
#endif

  return logger;
}

eError Logger::CreateClientLogger(const char* name, bool file_log, bool console_log) {
  if (m_client_logger = Logger::CreateLogger(name, file_log, console_log); !m_client_logger)
    return ERR_CANNOT_CREATE;

  return SUCCESS;
}

eError Logger::Create(bool file_logger, bool console_logger) {
  static Logger logger;

  if (s_singleton)
    return SUCCESS;

  spdlog::init_thread_pool(8192, 1);

  if (logger.m_engine_logger = Logger::CreateLogger("Engine", file_logger, console_logger); !logger.m_engine_logger)
    return ERR_CANNOT_CREATE;

  s_singleton = &logger;
  return SUCCESS;
}

void Logger::Shutdown() {
  m_engine_logger->flush();
  if (m_client_logger)
    m_client_logger->flush();

  spdlog::shutdown();
  m_client_logger.reset();
  m_engine_logger.reset();
  s_singleton = nullptr;
}
}  // namespace ye
