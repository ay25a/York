#include "core/assertion.hpp"
#include "core/logger.hpp"

using namespace ye;

int main() {
  if (auto res = Logger::Create(); res != ye::SUCCESS)
    YE_FATAL(std::format("Logger: {}", res));

  Logger::GetSingleton().Shutdown();
  return 0;
}
