#include "display_server.hpp"
#include "core/input_state.hpp"

namespace ye {
std::unique_ptr<DisplayServer> DisplayServer::s_singleton;
WindowID DisplayServer::s_next_window_id;

WindowID DisplayServer::RegisterWindow(void* handle, const WindowCreateInfo& ci) {
  m_windows[++s_next_window_id] = Window(handle, s_next_window_id, ci.title, ci.mode, vec2<uint16_t>(ci.width, ci.height), ci.flags);
  return s_next_window_id;
}

void* DisplayServer::UnregisterWindow(WindowID id) {
  YE_ASSERT(m_windows.contains(id), "Trying to unregister non-existant window id!");

  void* handle = m_windows[id].m_handle;
  m_windows.erase(id);
  return handle;
}

Window& DisplayServer::GetWindow(const WindowID& id) noexcept {
  YE_ASSERT(m_windows.contains(id), "Getting a window with non-existant ID");
  return m_windows[id];
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
}

eError DisplayServer::Create(std::unique_ptr<DisplayServer> ds) noexcept {
  YE_ASSERT(!s_singleton, "DisplayServer is already initialized!");
  YE_ASSERT(ds, "`nullptr` passed to DisplayServer::Create!");

  if (auto res = ds->Initialize(); res != SUCCESS)
    return res;

  s_singleton.swap(ds);
  s_singleton->CreateInputMap();
  return SUCCESS;
}

void DisplayServer::Shutdown() noexcept {
  auto clone = m_windows;
  for (const auto& [id, _] : clone)
    DestroyWindow(id);

  s_singleton.reset();
}

DisplayServer::~DisplayServer() noexcept {
  InputState::ResetState();
  s_next_window_id = WindowID::INVALID_ID();
}
}  // namespace ye
