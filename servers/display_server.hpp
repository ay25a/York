#pragma once

#include <cstdint>
#include "core/error_enum.hpp"
#include "core/input_enum.hpp"
#include "display/window.hpp"
#include "core/assertion.hpp"
#include <expected>
#include <memory>
#include <unordered_map>

namespace ye {
class DisplayServer {
 private:
  static WindowID s_next_window_id;
  std::unordered_map<WindowID, Window> m_windows;
  WindowID m_active_window = WINDOW_ID_INVALID;

 public:
  virtual std::expected<WindowID, eError> CreateWindow(const char* title, uint16_t width, uint16_t height, eWindowFlagBit flags) noexcept;
  virtual void DestroyWindow(WindowID id) noexcept;

  const Window& GetWindow(WindowID id) const noexcept;
  size_t GetWindowCount() const noexcept { return m_windows.size(); }
  bool IsWindowFocused(WindowID id) const noexcept { return m_active_window == id; };

  virtual void SetWindowTitle(WindowID id, std::string_view title) noexcept;
  virtual void ProcessEvents() noexcept;

 protected:
  void OnWindowFocus(WindowID id, bool is_focused) noexcept;
  void OnWindowModeChange(WindowID id, eWindowMode mode) noexcept;
  void OnWindowResize(WindowID id, uint16_t width, uint16_t height) noexcept;

  void OnKeyInput(eInputKey key, bool is_pressed) noexcept;
  void OnMouseMove(float window_x, float window_y) noexcept;
  void OnMouseScroll(int16_t delta_x, int16_t delta_y) noexcept;

 protected:
  DisplayServer() noexcept = default;
  static std::unique_ptr<DisplayServer> s_singleton;

 public:
  virtual ~DisplayServer() noexcept;

  static DisplayServer& GetSingleton() noexcept {
    YE_ASSERT(s_singleton, "DisplayServer is not initialized!");
    return *s_singleton;
  };

  void Shutdown() noexcept;
};

}  // namespace ye
