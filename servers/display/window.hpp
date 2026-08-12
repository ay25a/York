#pragma once

#include <cstdint>
#include "core/vec2.hpp"
#include <functional>
#include <string>

namespace ye {
// clang-format off
enum class eWindowMode { Windowed, Fullscreen, Maximized, Minimized };
enum eWindowFlagBit: uint16_t {
  WINDOW_FLAG_NO_RESIZE_BIT = 1 << 0,
  WINDOW_FLAG_MAX_BIT = 1 << 2,
};

using WindowFlags = uint16_t;
// clang-format on

struct WindowID {
  friend class DisplayServer;

  constexpr static WindowID INVALID_ID() { return WindowID(); }
  constexpr WindowID() noexcept = default;

  constexpr auto operator<=>(const WindowID&) const noexcept = default;
  constexpr int32_t Get() const noexcept { return id; }

 private:
  constexpr WindowID& operator++() noexcept {
    id++;
    return *this;
  }

  constexpr WindowID& operator--() noexcept {
    id--;
    return *this;
  }

 private:
  int32_t id = -1;
};

class Window {
  friend class DisplayServer;

 public:
  Window() = default;

 private:
  Window(void* handle, WindowID id, std::string_view title, eWindowMode mode, vec2<uint16_t> size, WindowFlags flags)
      : m_handle(handle), m_id(id), m_title(title), m_mode{mode}, m_size(size), m_flags(flags) {};

 public:
  void* GetHandle() const noexcept { return m_handle; }
  std::string_view GetTitle() const noexcept { return m_title; }
  eWindowMode GetMode() const noexcept { return m_mode; }
  vec2<uint16_t> GetSize() const noexcept { return m_size; }
  bool IsFocused() const noexcept { return m_is_focused; }
  bool HasFlag(eWindowFlagBit flag) const noexcept { return m_flags & flag; }

 private:
  void* m_handle = nullptr;
  WindowID m_id;
  std::string m_title;
  eWindowMode m_mode = eWindowMode::Windowed;
  vec2<uint16_t> m_size;
  bool m_is_focused = true;
  WindowFlags m_flags = eWindowFlagBit::WINDOW_FLAG_MAX_BIT;
};
}  // namespace ye

namespace std {
template <>
struct hash<ye::WindowID> {
  size_t operator()(const ye::WindowID& wid) const noexcept {
    return std::hash<decltype(wid.Get())>{}(wid.Get());
  }
};
};  // namespace std
