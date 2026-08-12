#include "display_server.hpp"
#include "core/input_state.hpp"

namespace ye {
std::unique_ptr<DisplayServer> DisplayServer::s_singleton;
WindowID DisplayServer::s_next_window_id;

std::expected<WindowID, eError> DisplayServer::CreateWindow(const WindowCreateInfo& ci) noexcept {
  auto handle = PlatformCreateWindow(ci);
  if (!handle)
    return std::unexpected(ERR_CANNOT_CREATE);

  m_windows[++s_next_window_id] = Window(handle.value(), s_next_window_id, ci.title, ci.mode, vec2<uint16_t>(ci.width, ci.height), ci.flags);
  return s_next_window_id;
}

void DisplayServer::DestroyWindow(const WindowID& id) noexcept {
  YE_ASSERT(m_windows.contains(id), "Trying to destroy a window with invalid ID");

  PlatformDestroyWindow(m_windows[id].m_handle);
  m_windows.erase(id);
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
  PlatformPollEvents();
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
  for (const auto& [_, window] : m_windows)
    PlatformDestroyWindow(window.m_handle);

  s_next_window_id = WindowID::INVALID_ID();
  s_singleton.reset();
  InputState::ResetState();
}
}  // namespace ye
