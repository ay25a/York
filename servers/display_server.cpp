#include "display_server.hpp"
#include "core/input_state.hpp"
#include "core/logger.hpp"

namespace ye {
std::unique_ptr<DisplayServer> DisplayServer::s_singleton;
WindowID DisplayServer::s_next_window_id = WINDOW_ID_INVALID;

WindowID DisplayServer::RegisterWindow(const WindowCreateInfo& ci) {
  m_windows[++s_next_window_id] = Window{
      .title = ci.title,
      .mode = ci.mode,
      .size = vec2<uint16_t>(ci.width, ci.height),
  };

  YE_ENGINE_INFO("Window registered [id: {}, title: {}]", s_next_window_id.get(), ci.title);
  return s_next_window_id;
}

void DisplayServer::RemoveWindow(const WindowID& id) {
  YE_ASSERT(m_windows.contains(id), "Attempting to unregister a non-existant window");

  m_windows.erase(id);
  YE_ENGINE_INFO("Window removed [id: {}]", s_next_window_id.get());
}

void DisplayServer::SetWindowTitle(const WindowID& id, std::string_view title) noexcept {
  YE_ASSERT(m_windows.contains(id), "Attempting to change a non-existant window title");
  m_windows[id].title = title;
}

const Window& DisplayServer::GetWindow(const WindowID& id) const noexcept {
  YE_ASSERT(m_windows.contains(id), "Attempting to access a non-existant window");
  return m_windows.at(id);
}

void DisplayServer::ProcessEvents() noexcept {
  InputState::ResetDeltas();
}

void DisplayServer::OnWindowFocused(const WindowID& id) noexcept {
  YE_ASSERT(m_windows.contains(id), "Attempting to set the focus on non-existant window");
  m_active_window = id;
}

void DisplayServer::OnWindowModeChange(const WindowID& id, eWindowMode mode) noexcept {
  YE_ASSERT(m_windows.contains(id), "Attemtping to set a mode for a non-existant window ");
  m_windows[id].mode = mode;
}

void DisplayServer::OnWindowResize(const WindowID& id, uint16_t width, uint16_t height) noexcept {
  YE_ASSERT(m_windows.contains(id), "Attemtping to set a size for a non-existant window");
  m_windows[id].size = vec2<uint16_t>(width, height);
}

void DisplayServer::OnKeyInput(const WindowID& id, eInputKey key, bool is_pressed) noexcept {
  YE_ASSERT(m_windows.contains(id), "Attempting to set key state associated with a non-existant window");
  InputState::UpdateKeyState(key, is_pressed);
}

void DisplayServer::OnMouseMove(const WindowID& id, float window_x, float window_y) noexcept {
  YE_ASSERT(m_windows.contains(id), "Attempting to set mouse position associated with a non-existant window");
  InputState::UpdateMousePosition(vec2<float>(window_x, window_y));
}

void DisplayServer::OnMouseScroll(const WindowID& id, int16_t delta_x, int16_t delta_y) noexcept {
  YE_ASSERT(m_windows.contains(id), "Attempting to set mouse axis associated with a non-existant window");
  InputState::UpdateMouseWheelDelta(vec2<int16_t>(delta_x, delta_y));
}

eError DisplayServer::Create(std::unique_ptr<DisplayServer> ds) noexcept {
  YE_ASSERT(!s_singleton, "DisplayServer is already initialized!");
  YE_ASSERT(ds, "`nullptr` passed to DisplayServer::Create!");

  if (auto res = ds->Initialize(); res != SUCCESS)
    return res;

  s_singleton.swap(ds);

  YE_ENGINE_INFO("DisplayServer created!");
  return SUCCESS;
}

void DisplayServer::Shutdown() noexcept {
  YE_ENGINE_INFO("DisplayServer destroyed!");
  s_singleton.reset();
}

DisplayServer::~DisplayServer() noexcept {
  InputState::ResetState();
  s_next_window_id = WINDOW_ID_INVALID;
}
}  // namespace ye
