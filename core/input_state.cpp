#include "input_state.hpp"

// clang-format off
std::bitset<static_cast<uint32_t>(ye::eInputKey::Count)>  ye::InputState::s_key_states;
vec2<uint16_t>                                            ye::InputState::s_mouse_position = {0, 0};
vec2<int16_t>                                             ye::InputState::s_mouse_position_delta = {0, 0};
vec2<int16_t>                                             ye::InputState::s_mouse_wheel_delta = {0, 0};
// clang-format on
