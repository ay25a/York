#pragma once

#include <cstdlib>
#include <print>

#if defined(_MSC_VER)
#define YE_FUNCTION __FUNCSIG__
#define YE_DEBUG_BREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
#define YE_FUNCTION __PRETTY_FUNCTION__
#define YE_DEBUG_BREAK() __builtin_trap()
#else
#include <csignal>
#define YE_FUNCTION __func__
#define YE_DEBUG_BREAK() raise(SIGTRAP)
#endif

#if defined(YE_DEBUG)
#define YE_ASSERT(cond, message)                                                                            \
  do {                                                                                                      \
    if (!(cond)) {                                                                                          \
      std::println(stderr, "ASSERTION FAILED!\nExpression: {}\nMessage: {}\nLocation: {}:{}\nFunction: {}", \
                   #cond, message, __FILE__, __LINE__, YE_FUNCTION);                                        \
      std::fflush(stderr);                                                                                  \
      YE_DEBUG_BREAK();                                                                                     \
      std::abort();                                                                                         \
    }                                                                                                       \
  } while (false)
#else
#define YE_ASSERT(...)
#endif

#define YE_FATAL(err)                                                                                                             \
  do {                                                                                                                            \
    if (::ye::Logger::Exist()) {                                                                                                  \
      ::ye::Logger::GetSingleton().GetEngineLogger().dump_backtrace();                                                            \
      ::ye::Logger::GetSingleton().GetEngineLogger().critical("{} occured at {}:{} at {}", err, __FILE__, __LINE__, YE_FUNCTION); \
      ::ye::Logger::GetSingleton().Shutdown();                                                                                    \
    } else                                                                                                                        \
      std::println(stderr, "FATAL ERROR: {} occured at {}:{} in {}", err, __FILE__, __LINE__, YE_FUNCTION);                       \
    std::abort();                                                                                                                 \
  } while (false)
