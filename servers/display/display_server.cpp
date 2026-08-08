#include "display_server.hpp"
#include "core/input_state.hpp"

namespace ye {
WindowID DisplayServer::s_next_window_id(0);

std::expected<WindowID, eError> DisplayServer::CreateWindow(std::string_view title, eWindowMode mode, uint16_t width, uint16_t height, uint32_t flags) noexcept {
  auto handle = CreateNativeWindow(title, mode, width, height, flags);
  if (!handle)
    return std::unexpected(ERR_CANNOT_CREATE);

  m_windows[s_next_window_id] = Window(handle.value(), s_next_window_id, title, mode, vec2<uint16_t>(width, height), flags);
  return s_next_window_id++;
}

void DisplayServer::DestroyWindow(const WindowID& id) noexcept {
  YE_ASSERT(m_windows.contains(id), "Trying to destroy a window with invalid ID");

  DestroyNativeWindow(m_windows[id].m_handle);
  m_windows.erase(id);
}

void DisplayServer::SetWindowMode(const WindowID& id, eWindowMode mode) noexcept {
  m_windows[id].m_mode = mode;
}

void DisplayServer::SetWindowFocus(const WindowID& id, bool is_focused) noexcept {
  m_windows[id].m_is_focused = is_focused;
}

void DisplayServer::SetWindowSize(const WindowID& id, vec2<uint16_t> size) noexcept {
  m_windows[id].m_size = size;
}

void DisplayServer::SetKeyState(const WindowID& id, eInputKey key, bool is_pressed) noexcept {
  InputState::UpdateKeyState(key, is_pressed);
}

void DisplayServer::SetMousePosition(const WindowID& id, const vec2<float>& pos) noexcept {
  InputState::UpdateMousePosition(pos);
}

void DisplayServer::SetMouseWheelDelta(const WindowID& id, vec2<int16_t> delta) noexcept {
  InputState::UpdateMouseWheelDelta(delta);
}

void DisplayServer::ProcessEvents() noexcept {
  InputState::ResetDeltas();
  PollNativeEvents();
}

void DisplayServer::Shutdown() {
  for (const auto& [_, window] : m_windows)
    DestroyNativeWindow(window.m_handle);

  m_windows.clear();
  s_next_window_id = 0;
}

}  // namespace ye
