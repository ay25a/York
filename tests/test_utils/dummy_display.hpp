#include <servers/display_server.hpp>
#include <core/logger.hpp>

namespace ye {

class DummyDisplay {
 private:
  class DummyServer : public DisplayServer {
   public:
    void InjectKeyEvent(const WindowID& id, eInputKey key, bool is_pressed) noexcept {
      DisplayServer::OnKeyInput(id, key, is_pressed);
    }

    struct WindowEvent {
      bool switch_focus = false;
      std::optional<vec2<uint16_t>> size;
      std::optional<eWindowMode> mode;
    };

    void InjectWindowEvent(const WindowID& id, const WindowEvent& event) noexcept {
      if (event.switch_focus)
        DisplayServer::OnWindowFocused(id);

      if (event.mode.has_value())
        DisplayServer::OnWindowModeChange(id, event.mode.value());

      if (event.size.has_value())
        DisplayServer::OnWindowResize(id, event.size->x, event.size->y);
    }

    struct MouseEvent {
      std::optional<vec2<float>> position;
      std::optional<vec2<int16_t>> scroll_delta;
    };

    void InjectMouseEvent(const WindowID& id, const MouseEvent& event) noexcept {
      if (event.position.has_value())
        DisplayServer::OnMouseMove(id, event.position->x, event.position->y);

      if (event.scroll_delta.has_value())
        DisplayServer::OnMouseScroll(id, event.scroll_delta->x, event.scroll_delta->y);
    }

   public:
    std::expected<WindowID, eError> CreateWindow(const WindowCreateInfo& ci) noexcept override { return RegisterWindow(ci); }
    void DestroyWindow(const WindowID& id) noexcept override { RemoveWindow(id); }

    virtual ~DummyServer() noexcept override = default;

   private:
    eError Initialize() noexcept override { return SUCCESS; }
  };

 public:
  DummyDisplay() {
    if (Logger::Create(false, false) != SUCCESS)
      YE_FATAL("Logger cannot be created!");

    auto res = DisplayServer::Create(std::make_unique<DummyServer>());
    YE_ASSERT(res == SUCCESS, "Display Test Failed! Cannot create display server");
  }

  ~DummyDisplay() {
    DisplayServer::GetSingleton().Shutdown();
  }

  DummyServer& GetServer() {
    return *static_cast<DummyServer*>(&DisplayServer::GetSingleton());
  };
};
}  // namespace ye
