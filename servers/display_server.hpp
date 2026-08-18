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
  struct WindowCreateInfo {
    std::string title;
    eWindowMode mode = eWindowMode::Windowed;
    uint16_t width = 1080;
    uint16_t height = 1920;
    WindowFlags flags = WINDOW_FLAG_NONE;
  };

  virtual std::expected<WindowID, eError> CreateWindow(const WindowCreateInfo& ci) noexcept = 0;
  virtual void DestroyWindow(const WindowID& id) noexcept = 0;

  virtual void SetWindowTitle(const WindowID& id, std::string_view title) noexcept;

  const Window& GetWindow(const WindowID& id) const noexcept;
  size_t GetWindowCount() const noexcept { return m_windows.size(); }

  virtual void ProcessEvents() noexcept;
  bool IsWindowFocused(const WindowID& id) const noexcept { return m_active_window == id; };

 protected:
  WindowID RegisterWindow(const WindowCreateInfo& ci);
  void RemoveWindow(const WindowID& id);

  void OnWindowFocus(const WindowID& id, bool is_focused) noexcept;
  void OnWindowModeChange(const WindowID& id, eWindowMode mode) noexcept;
  void OnWindowResize(const WindowID& id, uint16_t width, uint16_t height) noexcept;

  void OnKeyInput(const WindowID& id, eInputKey key, bool is_pressed) noexcept;
  void OnMouseMove(const WindowID& id, float window_x, float window_y) noexcept;
  void OnMouseScroll(const WindowID& id, int16_t delta_x, int16_t delta_y) noexcept;

 private:
  static std::unique_ptr<DisplayServer> s_singleton;
  virtual eError Initialize() noexcept = 0;

 public:
  DisplayServer() noexcept = default;
  virtual ~DisplayServer() noexcept;

  static DisplayServer& GetSingleton() noexcept {
    YE_ASSERT(s_singleton, "DisplayServer is not initialized!");
    return *s_singleton;
  };

  static eError Create(std::unique_ptr<DisplayServer> ds) noexcept;
  void Shutdown() noexcept;
};

}  // namespace ye
