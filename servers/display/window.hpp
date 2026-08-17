#pragma once

#include <cstdint>
#include "core/vec2.hpp"
#include <string>
#include "window_id.hpp"

namespace ye {
constexpr WindowID WINDOW_ID_INVALID = WindowID();

// clang-format off
enum class eWindowMode { Windowed, Fullscreen, Maximized, Minimized };
enum eWindowFlagBit {};
using WindowFlags = uint16_t;
constexpr WindowFlags WINDOW_FLAG_NONE = 0;
// clang-format on

struct Window {
  std::string title;
  eWindowMode mode = eWindowMode::Windowed;
  vec2<uint16_t> size;
};
}  // namespace ye
