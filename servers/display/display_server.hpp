#pragma once

#include <cstdint>
#include "core/error_enum.hpp"
#include "core/input_enum.hpp"
#include "platform_input.hpp"
#include "window.hpp"
#include "core/assertion.hpp"
#include <expected>
#include <unordered_map>

namespace ye {
class DisplayServer {
 private:
  static WindowID s_next_window_id;
  std::unordered_map<WindowID, Window> m_windows;

 public:
  virtual std::expected<void*, eError> CreateNativeWindow(std::string_view title, eWindowMode mode, uint16_t width, uint16_t height, uint32_t flags) noexcept = 0;
  virtual void DestroyNativeWindow(void* handle) noexcept = 0;

  std::expected<WindowID, eError> CreateWindow(std::string_view title, eWindowMode mode, uint16_t width, uint16_t height, uint32_t flags) noexcept;
  void DestroyWindow(const WindowID& id) noexcept;

  const Window& GetWindow(const WindowID& id) const noexcept {
    YE_ASSERT(m_windows.contains(id), "Trying to access a window with an invalid ID!");
    return m_windows[id];
  }

 private:
  void SetWindowMode(const WindowID& id, eWindowMode mode) noexcept;
  void SetWindowSize(const WindowID& id, vec2<uint16_t> size) noexcept;
  void SetWindowFocus(const WindowID& id, bool is_focused) noexcept;
  void SetKeyState(const WindowID& id, eInputKey key, bool isPressed) noexcept;
  void SetMousePosition(const WindowID& id, const vec2<float>& pos) noexcept;
  void SetMouseWheelDelta(const WindowID& id, vec2<int16_t> delta) noexcept;

  virtual void PollNativeEvents() const noexcept = 0;

 public:
  void ProcessEvents() noexcept;

 private:
  DisplayServer* s_singleton = nullptr;
  DisplayServer() = default;
  ~DisplayServer() = default;

 public:
  inline DisplayServer& GetSingleton() {
    YE_ASSERT(s_singleton, "DisplayServer is not initialized!");
    return *s_singleton;
  };

  virtual eError Create() = 0;
  void Shutdown();
};

}  // namespace ye
