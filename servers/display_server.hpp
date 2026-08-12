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
 public:
  struct WindowCreateInfo {
    std::string title;
    eWindowMode mode = eWindowMode::Windowed;
    uint16_t width = 1080;
    uint16_t height = 1920;
    WindowFlags flags = eWindowFlagBit::WINDOW_FLAG_MAX_BIT;
  };

 private:
  std::unordered_map<WindowID, Window> m_windows;
  static WindowID s_next_window_id;

  virtual std::expected<void*, eError> PlatformCreateWindow(const WindowCreateInfo& ci) noexcept = 0;
  virtual void PlatformDestroyWindow(void* handle) noexcept = 0;

  virtual void CreateInputMap() noexcept = 0;
  virtual void PlatformPollEvents() const noexcept = 0;

 public:
  std::expected<WindowID, eError> CreateWindow(const WindowCreateInfo& ci) noexcept;
  void DestroyWindow(const WindowID& id) noexcept;

  Window& GetWindow(const WindowID& id) noexcept;
  size_t GetWindowCount() const noexcept { return m_windows.size(); }

  void ProcessEvents() noexcept;

 protected:
  std::unordered_map<uint32_t, eInputKey> m_input_map;

  void SetWindowMode(const WindowID& id, eWindowMode mode) noexcept;
  void SetWindowSize(const WindowID& id, vec2<uint16_t> size) noexcept;
  void SetWindowFocus(const WindowID& id, bool is_focused) noexcept;
  void SetKeyState(const WindowID& id, uint32_t native_key, bool is_pressed) noexcept;
  void SetMousePosition(const WindowID& id, const vec2<float>& pos) noexcept;
  void SetMouseWheelDelta(const WindowID& id, vec2<int16_t> delta) noexcept;

 private:
  static std::unique_ptr<DisplayServer> s_singleton;
  virtual eError Initialize() noexcept = 0;

 public:
  DisplayServer() noexcept = default;
  virtual ~DisplayServer() noexcept = default;

  static DisplayServer& GetSingleton() noexcept {
    YE_ASSERT(s_singleton, "DisplayServer is not initialized!");
    return *s_singleton;
  };

  static eError Create(std::unique_ptr<DisplayServer> ds) noexcept;
  virtual void Shutdown() noexcept;
};

}  // namespace ye
