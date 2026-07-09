#include "core/assertion.hpp"
#define YE_LOGGER_CLIENT_MACROS
#include "core/logger.hpp"

using namespace ye;
int main() {
  if (Logger::Create() != ye::SUCCESS)
    YE_FATAL("Logger cannot be created!");

  YE_ENGINE_ERROR("ERROR");
  YE_ENGINE_WARN("WARNING");
  YE_ENGINE_INFO("INFO");

  if (Logger::GetSingleton().CreateClientLogger("Client", false, true) != SUCCESS)
    YE_FATAL("Client logger cannot be created!");

  YE_CLIENT_ERROR("ERROR");
  YE_CLIENT_WARN("WARNING");
  YE_CLIENT_INFO("INFO");

  YE_FATAL("Error");

  return 0;
}
