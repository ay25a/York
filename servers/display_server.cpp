#include "display_server.hpp"
#include "core/input_state.hpp"
#include "core/logger.hpp"

namespace ye {
std::unique_ptr<DisplayServer> DisplayServer::s_singleton;
WindowID DisplayServer::s_next_window_id = WINDOW_ID_INVALID;

std::expected<WindowID, eError> DisplayServer::CreateWindow(const char* title, uint16_t width, uint16_t height, eWindowFlagBit flags) noexcept {
  YE_ENGINE_INFO("CreateWindow [id: {}, title: {}]", s_next_window_id, title);
  m_windows[++s_next_window_id] = Window{
      .title = title,
      .size = vec2<uint16_t>(width, height),
      .mode = eWindowMode::Windowed,
      .flags = flags,
  };

  m_active_window = s_next_window_id;
  return s_next_window_id;
}

void DisplayServer::DestroyWindow(WindowID id) noexcept {
  YE_ASSERT(m_windows.contains(id), "window doesn't exist");

  m_windows.erase(id);
  YE_ENGINE_INFO("DestroyWindow [id: {}]", s_next_window_id);
}

void DisplayServer::SetWindowTitle(WindowID id, std::string_view title) noexcept {
  YE_ASSERT(m_windows.contains(id), "invalid window id");
  m_windows[id].title = title;
}

const Window& DisplayServer::GetWindow(WindowID id) const noexcept {
  YE_ASSERT(m_windows.contains(id), "invalid window id");
  return m_windows.at(id);
}

void DisplayServer::ProcessEvents() noexcept {
  InputState::ResetDeltas();
}

void DisplayServer::OnWindowFocus(WindowID id, bool is_focused) noexcept {
  if (is_focused)
    m_active_window = id;
  else if (m_active_window == id)
    m_active_window = WINDOW_ID_INVALID;
}

void DisplayServer::OnWindowModeChange(WindowID id, eWindowMode mode) noexcept {
  m_windows[id].mode = mode;
}

void DisplayServer::OnWindowResize(WindowID id, uint16_t width, uint16_t height) noexcept {
  m_windows[id].size = vec2<uint16_t>(width, height);
}

void DisplayServer::OnKeyInput(eInputKey key, bool is_pressed) noexcept {
  InputState::UpdateKeyState(key, is_pressed);
}

void DisplayServer::OnMouseMove(float window_x, float window_y) noexcept {
  InputState::UpdateMousePosition(vec2<float>(window_x, window_y));
}

void DisplayServer::OnMouseScroll(int16_t delta_x, int16_t delta_y) noexcept {
  InputState::UpdateMouseWheelDelta(vec2<int16_t>(delta_x, delta_y));
}

void DisplayServer::Shutdown() noexcept {
  s_singleton.reset();
  YE_ENGINE_INFO("DisplayServer::Shutdown!");
}

DisplayServer::~DisplayServer() noexcept {
  InputState::ResetState();
  s_next_window_id = WINDOW_ID_INVALID;
}
}  // namespace ye
