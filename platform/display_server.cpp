#include "display_server.hpp"
#include "core/input_state.hpp"

namespace ye {
WindowID DisplayServer::s_next_window_id(0);

std::expected<WindowID, eError> DisplayServer::CreateWindow(std::string_view title, vec2<uint16_t> requested_size) {
  auto handle = CreateNativeWindow(title, requested_size);
  if (!handle)
    return std::unexpected(ERR_CANNOT_CREATE);

  m_native_handles[s_next_window_id] = handle.value();
  m_windows[s_next_window_id] = Window{
      .id = s_next_window_id,
      .title = title.data(),
      .size = requested_size,
  };

  return s_next_window_id++;
}

void DisplayServer::DestroyWindow(const WindowID& id) {
  YE_ASSERT(m_native_handles.contains(id), "Trying to destroy a window with invalid ID");

  DestroyNativeWindow(m_native_handles[id]);

  m_native_handles.erase(id);
  m_windows.erase(id);
}

void DisplayServer::OnStateChange(const WindowID& id, eWindowState new_state) {
  m_windows[id].state = new_state;
}

void DisplayServer::OnFocusChange(const WindowID& id, bool is_focused) {
  m_windows[id].is_focused = is_focused;
}

void DisplayServer::OnSizeChange(const WindowID& id, const vec2<uint16_t>& new_size) {
  m_windows[id].size = new_size;
}

void DisplayServer::OnKeyInput(eInputKey key, bool isPressed) {
  InputState::SetKeyState(key, isPressed);
}

void DisplayServer::OnMouseScroll(const vec2<int16_t>& delta) {
  InputState::SetMouseWheelDelta(delta);
}

void DisplayServer::OnMouseMove(vec2<uint16_t> new_pos) {
  InputState::SetMousePosition(new_pos);
}

void DisplayServer::ProcessEvents() {
  InputState::BeginFrame();
  PollNativeEvents();
}

void DisplayServer::Shutdown() {
  for (auto& [id, handle] : m_native_handles)
    DestroyNativeWindow(handle);

  m_native_handles.clear();
  m_windows.clear();
}

}  // namespace ye
