#pragma once

#include <format>

namespace ye {
enum eError {
  SUCCESS = 0,
  ERR_CANNOT_CREATE,
};

inline constexpr const char* ErrorToString(eError err) {
  switch (err) {
    // clang-format off
  case SUCCESS:             return "Success";
  case ERR_CANNOT_CREATE:   return "Cannot Create";
    // clang-format on
  }
}
};  // namespace ye

template <>
struct std::formatter<ye::eError> : std::formatter<std::string> {
  auto format(const ye::eError& err, std::format_context& ctx) const {
    return std::format_to(ctx.out(), "{}", ye::ErrorToString(err));
  }
};
