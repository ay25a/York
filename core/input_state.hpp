#pragma once

#include "input_enum.hpp"
#include "vec2.hpp"
#include <bitset>
#include <cstdint>

namespace ye {
class InputState {
  friend class DisplayServer;

 public:
  static bool IsPressed(eInputKey key) noexcept { return s_keys_state.test(static_cast<uint32_t>(key)); }
  static vec2<float> GetMousePosition() noexcept { return s_mouse_position; }
  static vec2<float> GetMousePositionDelta() noexcept { return s_mouse_position_delta; }
  static vec2<int16_t> GetMouseWheelDelta() noexcept { return s_mouse_wheel_delta; }

 private:
  static void ResetDeltas() noexcept;
  static void ResetState() noexcept;

  static void UpdateKeyState(eInputKey key, bool is_pressed) noexcept {
    s_keys_state.set(static_cast<uint32_t>(key), is_pressed);
  }

  static void UpdateMousePosition(vec2<float> new_pos) noexcept {
    s_mouse_position_delta = new_pos - s_mouse_position;
    s_mouse_position = new_pos;
  }

  static void UpdateMouseWheelDelta(vec2<int16_t> new_delta) noexcept {
    s_mouse_wheel_delta = new_delta;
  }

 private:
  static constexpr uint32_t ALIGNED_INPUT_COUNT = (static_cast<uint32_t>(eInputKey::Count) + 127) & ~127;

  static std::bitset<ALIGNED_INPUT_COUNT> s_keys_state;
  static vec2<float> s_mouse_position;
  static vec2<float> s_mouse_position_delta;
  static vec2<int16_t> s_mouse_wheel_delta;
};
}  // namespace ye
