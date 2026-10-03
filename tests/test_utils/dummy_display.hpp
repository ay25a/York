#include <servers/display_server.hpp>
#include <core/logger.hpp>

namespace ye {

class DummyDisplayServer : public DisplayServer {
  Monitor m_monitor;

 public:
  virtual const Monitor& GetMonitor() override { return m_monitor; }

 public:
  void InjectKeyEvent(eInputKey key, bool is_pressed) noexcept { DisplayServer::OnKeyInput(key, is_pressed); }

  struct WindowEvent {
    std::optional<bool> focus;
    std::optional<vec2<uint16_t>> size;
    std::optional<eWindowMode> mode;
  };

  void InjectWindowEvent(WindowID id, const WindowEvent& event) noexcept {
    if (event.focus.has_value())
      DisplayServer::OnWindowFocus(id, event.focus.value());

    if (event.mode.has_value())
      DisplayServer::OnWindowModeChange(id, event.mode.value());

    if (event.size.has_value())
      DisplayServer::OnWindowResize(id, event.size->x, event.size->y);
  }

  struct MouseEvent {
    std::optional<vec2<float>> position;
    std::optional<vec2<int16_t>> scroll_delta;
  };

  void InjectMouseEvent(const MouseEvent& event) noexcept {
    if (event.position.has_value())
      DisplayServer::OnMouseMove(event.position->x, event.position->y);

    if (event.scroll_delta.has_value())
      DisplayServer::OnMouseScroll(event.scroll_delta->x, event.scroll_delta->y);
  }

 public:
  DummyDisplayServer() {
    if (Logger::Create(false, false) != SUCCESS)
      YE_FATAL("Logger cannot be created!");
  }

  ~DummyDisplayServer() {
    DisplayServer::Shutdown();
  }
};
}  // namespace ye
