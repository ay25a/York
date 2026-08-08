#include "input_state.hpp"

// clang-format off
std::bitset<static_cast<uint32_t>(ye::eInputKey::Count)>  ye::InputState::s_keys_state;
vec2<float>                                               ye::InputState::s_mouse_position;
vec2<float>                                               ye::InputState::s_mouse_position_delta;
vec2<int16_t>                                             ye::InputState::s_mouse_wheel_delta;
// clang-format on

namespace ye {
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
