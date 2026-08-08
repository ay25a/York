#pragma once

#include <cstdint>
#include "core/vec2.hpp"
#include <functional>
#include <string>

namespace ye {
// clang-format off
enum class eWindowMode { Windowed, Fullscreen, Maximized, Minimized };
enum eWindowFlagBit: uint32_t {};
// clang-format on

struct WindowID {
  int32_t value = 0;

  WindowID(int32_t value) : value(value) {}
  WindowID() = default;
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

class Window {
  friend class DisplayServer;

 public:
  static constexpr uint32_t INVALID_ID = UINT32_MAX;

 private:
  Window(void* handle, WindowID id, std::string_view title, eWindowMode mode, vec2<uint16_t> size, uint32_t flags)
      : m_handle(handle), m_id(id), m_title(title), m_mode{mode}, m_size(size), m_flags(flags) {};

 public:
  void* GetHandle() const noexcept { return m_handle; }
  std::string_view GetTitle() const noexcept { return m_title; }
  eWindowMode GetMode() const noexcept { return m_mode; }
  vec2<uint16_t> GetSize() const noexcept { return m_size; }
  bool IsFocused() const noexcept { return m_is_focused; }
  bool HasFlag(eWindowFlagBit flag) { return m_flags & flag; }

 private:
  void* m_handle = nullptr;
  WindowID m_id = INVALID_ID;
  std::string m_title;
  eWindowMode m_mode = eWindowMode::Windowed;
  vec2<uint16_t> m_size;
  bool m_is_focused = true;
  uint32_t m_flags = 0;
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
