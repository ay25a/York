#pragma once

#include "input.hpp"
#include "vec2.hpp"
#include <bitset>
#include <cstdint>

namespace ye {
class InputState {
  friend class DisplayServer;
  /// TODO: Remove when `DisplayServer` is added
  friend class InputStateTestAccessor;

 public:
  static inline void BeginFrame() {
    s_mouse_position_delta = {};
    s_mouse_wheel_delta = {};
  }

  static inline void Reset() {
    s_key_states.reset();
    s_mouse_position = {};
    s_mouse_position_delta = {};
    s_mouse_wheel_delta = {};
  }

  static bool IsPressed(eInputKey key) { return s_key_states.test(static_cast<uint32_t>(key)); }
  static inline auto GetMousePosition() noexcept { return s_mouse_position; }
  static inline auto GetMousePositionDelta() noexcept { return s_mouse_position_delta; }
  static inline auto GetMouseWheelDelta() noexcept { return s_mouse_wheel_delta; }

 private:
  static inline void SetKeyState(eInputKey key, bool isPressed) {
    s_key_states.set(static_cast<uint32_t>(key), isPressed);
  }
  static inline void SetMousePosition(const vec2<uint16_t> new_pos) {
    s_mouse_position_delta = {static_cast<int16_t>(new_pos.x - s_mouse_position.x), static_cast<int16_t>(new_pos.y - s_mouse_position.y)};
    s_mouse_position = new_pos;
  }

  static inline void SetMouseWheelDelta(const vec2<int16_t> new_delta) { s_mouse_wheel_delta = new_delta; }

 private:
  static std::bitset<static_cast<uint32_t>(eInputKey::Count)> s_key_states;
  static vec2<uint16_t> s_mouse_position;
  static vec2<int16_t> s_mouse_position_delta;
  static vec2<int16_t> s_mouse_wheel_delta;
};
}  // namespace ye
