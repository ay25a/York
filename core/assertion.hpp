#pragma once

#include <cstdlib>

#if defined(YE_REL_WITH_DEB_INFO) || defined(YE_DEBUG)
#include <print>

#if defined(_MSC_VER)
#define YE_DEBUG_BREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
#define YE_DEBUG_BREAK() __builtin_trap()
#else
#include <csignal>
#define YE_DEBUG_BREAK() raise(SIGTRAP)
#endif

#define YE_ASSERT(cond, message)                                                                  \
  do {                                                                                            \
    if (cond) continue;                                                                           \
    std::println("ASSERTION FAILED!\nExpression: {}\nMessage: {}\nLocation: {}:{}\nFunction: {}", \
                 #cond, message, __FILE__, __LINE__, __PRETTY_FUNCTION__);                        \
    YE_DEBUG_BREAK();                                                                             \
    std::abort();                                                                                 \
  } while (false);
#else
#define YE_ASSERT(...)
#endif

#define YE_FATAL(...)                                                  \
  do {                                                                 \
    ye::Logger::GetSingleton()->GetEngineLogger()->error(__VA_ARGS__); \
    ye::Logger::GetSingleton()->Shutdown();                            \
    std::abort();                                                      \
  } while (false);

// To satisfy the compiler
namespace {}
