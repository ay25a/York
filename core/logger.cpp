#include "logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>
#include <spdlog/async_logger.h>

namespace ye {
Logger* Logger::s_singleton = nullptr;

Logger* Logger::GetSingleton() {
  return Logger::s_singleton;
}

Logger* Logger::Create() {
  static Logger logger;

  if (s_singleton)
    return s_singleton;

  logger.Startup();
  s_singleton = &logger;
  return s_singleton;
}

void Logger::Startup() {
  spdlog::init_thread_pool(8192, 1);
  auto stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

  std::vector<spdlog::sink_ptr> sinks{stdoutSink};
  m_pEngine_logger = std::make_shared<spdlog::async_logger>("Engine", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
  m_pEngine_logger = std::make_shared<spdlog::async_logger>("Engine", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
}
}  // namespace ye
