#pragma once

#include <cstdint>
#include "core/vec2.hpp"
#include <string>

namespace ye {
typedef int WindowID;

constexpr WindowID WINDOW_ID_INVALID = -1;

enum class eWindowMode {
  Windowed,
  Fullscreen,
  Maximized,
  Minimized
};

enum eWindowFlagBit : uint16_t {
  WINDOW_FLAG_NONE = 0,
};

struct Window {
  std::string title;
  vec2<uint16_t> size;
  eWindowMode mode = eWindowMode::Windowed;
  eWindowFlagBit flags = WINDOW_FLAG_NONE;
};

struct Monitor {
  uint32_t id = 0;
  std::string name;

  uint32_t width = 0;
  uint32_t height = 0;

  float scale = .0f;
  float refresh_rate = .0f;
};
}  // namespace ye
