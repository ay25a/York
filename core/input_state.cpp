#include "input_state.hpp"

namespace ye {
// clang-format off
std::bitset<InputState::ALIGNED_INPUT_COUNT>  InputState::s_keys_state;
vec2<float>                                   InputState::s_mouse_position;
vec2<float>                                   InputState::s_mouse_position_delta;
vec2<int16_t>                                 InputState::s_mouse_wheel_delta;
// clang-format on

void InputState::ResetDeltas() noexcept {
  s_mouse_position_delta = {.0f, .0f};
  s_mouse_wheel_delta = {0, 0};
}

void InputState::ResetState() noexcept {
  s_keys_state.reset();
  s_mouse_position = {.0f, .0f};
  s_mouse_position_delta = {.0f, .0f};
  s_mouse_wheel_delta = {0, 0};
}
}  // namespace ye
