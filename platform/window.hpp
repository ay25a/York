#pragma once

#include <cstdint>
#include <compare>
#include "core/vec2.hpp"
#include <functional>
#include <string>

namespace ye {
// clang-format off
enum class eWindowState { Normal = 0, Fullscreen, Maximized };
// clang-format on

constexpr uint32_t INVALID_WINDOW_ID = UINT32_MAX;
struct WindowID {
  uint32_t value = INVALID_WINDOW_ID;

  WindowID() = default;
  WindowID(uint32_t id) : value(id) {}

  ~WindowID() = default;

  auto operator<=>(const WindowID&) const = default;
  WindowID& operator++() {
    value++;
    return *this;
  }

  WindowID& operator--() {
    value--;
    return *this;
  }

  WindowID operator++(int) {
    WindowID old = *this;
    ++value;
    return old;
  }

  WindowID operator--(int) {
    WindowID old = *this;
    --value;
    return old;
  }
};

struct Window {
  WindowID id;
  std::string title;
  eWindowState state = eWindowState::Normal;
  vec2<uint16_t> size;
  bool is_focused = true;
};
}  // namespace ye

namespace std {
template <>
struct hash<ye::WindowID> {
  size_t operator()(const ye::WindowID& id) const noexcept {
    return std::hash<decltype(id.value)>{}(id.value);
  }
};
};  // namespace std
