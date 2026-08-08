#pragma once

#include <cstdint>
#include "core/error_enum.hpp"
#include "core/input.hpp"
#include "platform/platform_input.hpp"
#include "platform/window.hpp"
#include "core/assertion.hpp"
#include <expected>
#include <unordered_map>

namespace ye {
class DisplayServer {
 private:
  static WindowID s_next_window_id;
  std::unordered_map<WindowID, void*> m_native_handles;
  std::unordered_map<WindowID, Window> m_windows;

  virtual std::expected<void*, eError> CreateNativeWindow(std::string_view title, vec2<uint16_t> size) = 0;
  virtual void DestroyNativeWindow(void* handle) = 0;

 public:
  std::expected<WindowID, eError> CreateWindow(std::string_view title, vec2<uint16_t> requested_size);
  void DestroyWindow(const WindowID& id);

  Window& GetWindow(const WindowID& id) {
    YE_ASSERT(m_windows.contains(id), "Trying to access a window with an invalid ID!");
    return m_windows[id];
  }

  void* GetNativeHandle(const WindowID& id) {
    YE_ASSERT(m_native_handles.contains(id), "Trying to access a window with an invalid ID!");
    return m_native_handles[id];
  }

 private:
  PlatformInput* m_platform_input = nullptr;
  virtual void PollNativeEvents() = 0;

  void OnStateChange(const WindowID& id, eWindowState new_state);
  void OnFocusChange(const WindowID& id, bool is_focused);
  void OnSizeChange(const WindowID& id, const vec2<uint16_t>& new_size);
  void OnKeyInput(eInputKey key, bool isPressed);
  void OnMouseScroll(const vec2<int16_t>& delta);
  void OnMouseMove(vec2<uint16_t> new_pos);

 public:
  void ProcessEvents();

 private:
  DisplayServer* s_singleton;
  DisplayServer() = default;
  ~DisplayServer();

 public:
  inline DisplayServer& GetSingleton() {
    YE_ASSERT(s_singleton, "DisplayServer is not initialized!");
    return *s_singleton;
  };

  virtual eError Create() = 0;
  void Shutdown();
};

}  // namespace ye
