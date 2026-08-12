#include <servers/display_server.hpp>

namespace ye {

class DummyDisplay {
 private:
  class DummyServer : public DisplayServer {
   public:
    virtual ~DummyServer() noexcept override = default;

   private:
    std::expected<void*, eError> PlatformCreateWindow(const WindowCreateInfo& ci) noexcept override { return nullptr; }
    void PlatformDestroyWindow(void* handle) noexcept override {}

    void CreateInputMap() noexcept override {
      for (size_t i = 0; i < static_cast<uint32_t>(eInputKey::Count); ++i) {
        m_input_map[i] = static_cast<eInputKey>(i);
      }
    }

    void PlatformPollEvents() const noexcept override {};
    eError Initialize() noexcept override { return SUCCESS; }

   public:
    void InjectKeyEvent(eInputKey key, bool is_pressed) noexcept {
      DisplayServer::SetKeyState(WindowID::INVALID_ID(), static_cast<uint32_t>(key), is_pressed);
    }

    struct WindowEvent {
      std::optional<bool> is_focused;
      std::optional<vec2<uint16_t>> size;
      std::optional<eWindowMode> mode;
    };

    void InjectWindowEvent(const WindowEvent& event) noexcept {
      if (event.is_focused.has_value())
        DisplayServer::SetWindowFocus(WindowID::INVALID_ID(), event.is_focused.value());

      if (event.mode.has_value())
        DisplayServer::SetWindowMode(WindowID::INVALID_ID(), event.mode.value());

      if (event.size.has_value())
        DisplayServer::SetWindowSize(WindowID::INVALID_ID(), event.size.value());
    }

    struct MouseEvent {
      std::optional<vec2<float>> position;
      std::optional<vec2<int16_t>> scroll_delta;
    };

    void InjectMouseEvent(const MouseEvent& event) noexcept {
      if (event.position.has_value())
        DisplayServer::SetMousePosition(WindowID::INVALID_ID(), event.position.value());

      if (event.scroll_delta.has_value())
        DisplayServer::SetMouseWheelDelta(WindowID::INVALID_ID(), event.scroll_delta.value());
    }
  };

 public:
  DummyDisplay() {
    YE_ASSERT(DisplayServer::Create(std::make_unique<DummyServer>()) == SUCCESS, "Display Test Failed! Cannot create display server");
  }

  ~DummyDisplay() {
    DisplayServer::GetSingleton().Shutdown();
  }

  DummyServer* GetServer() {
    return static_cast<DummyServer*>(&DisplayServer::GetSingleton());
  };
};
}  // namespace ye
