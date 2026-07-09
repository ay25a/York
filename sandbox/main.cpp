#include "core/assertion.hpp"
#include "core/logger.hpp"

using namespace ye;
int main() {
  if (Logger::Create() != ye::SUCCESS)
    YE_FATAL("Logger cannot be created!");

  YE_ENGINE_ERROR("ERR");

  Logger::GetSingleton().Shutdown();
  return 0;
}
