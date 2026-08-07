/// TODO: Reroute the tests thorugh DisplayServer
#include <gtest/gtest.h>
#include <core/input_state.hpp>

namespace ye {
class InputStateTestAccessor {
 public:
  static void PressKey(eInputKey key) {
    InputState::SetKeyState(key, true);
  }

  static void ReleaseKey(eInputKey key) {
    InputState::SetKeyState(key, false);
  }

  static void MoveMouse(vec2<uint16_t> pos) {
    InputState::SetMousePosition(pos);
  }

  static void ScrollMouse(vec2<int16_t> delta) {
    InputState::SetMouseWheelDelta(delta);
  }
};
}  // namespace ye

TEST(InputStateTest, CorrectMouseAndKeyboardState) {
  ye::InputState::Reset();

  ye::InputStateTestAccessor::PressKey(ye::eInputKey::MouseRight);
  EXPECT_TRUE(ye::InputState::IsPressed(ye::eInputKey::MouseRight));
  EXPECT_FALSE(ye::InputState::IsPressed(ye::eInputKey::MouseMiddle));

  vec2<uint16_t> mouse_pos = {102, 200};
  ye::InputStateTestAccessor::MoveMouse(mouse_pos);
  EXPECT_EQ(ye::InputState::GetMousePosition() == mouse_pos, true);

  vec2<int16_t> wheel_delta = {-3, 3};
  ye::InputStateTestAccessor::ScrollMouse(wheel_delta);
  EXPECT_EQ(ye::InputState::GetMouseWheelDelta(), wheel_delta);
}

TEST(InputStateTest, CorrectMouseDelta) {
  ye::InputState::Reset();

  vec2<uint16_t> mouse_pos = {102, 200};
  vec2<int16_t> expected_delta = {
      static_cast<int16_t>(mouse_pos.x - ye::InputState::GetMousePosition().x),
      static_cast<int16_t>(mouse_pos.y - ye::InputState::GetMousePosition().y),
  };

  ye::InputStateTestAccessor::MoveMouse(mouse_pos);
  EXPECT_EQ(ye::InputState::GetMousePositionDelta(), expected_delta);

  mouse_pos = {948, 500};
  expected_delta = {
      static_cast<int16_t>(mouse_pos.x - ye::InputState::GetMousePosition().x),
      static_cast<int16_t>(mouse_pos.y - ye::InputState::GetMousePosition().y),
  };
  ye::InputStateTestAccessor::MoveMouse(mouse_pos);
  EXPECT_EQ(ye::InputState::GetMousePositionDelta(), expected_delta);
}

TEST(InputStateTest, CorrectFrameResetAndGlobalReset) {
  ye::InputState::Reset();

  vec2<uint16_t> empty_vec_u{0, 0};
  vec2<int16_t> empty_vec_i{0, 0};

  EXPECT_EQ(ye::InputState::GetMousePosition(), empty_vec_u);
  EXPECT_EQ(ye::InputState::GetMousePositionDelta(), empty_vec_i);
  EXPECT_EQ(ye::InputState::GetMouseWheelDelta(), empty_vec_i);
  EXPECT_EQ(ye::InputState::IsPressed(ye::eInputKey::AltRight), false);

  ye::InputStateTestAccessor::MoveMouse({102, 102});
  EXPECT_NE(ye::InputState::GetMousePosition(), empty_vec_u);

  ye::InputStateTestAccessor::ScrollMouse({-23, 20});
  EXPECT_NE(ye::InputState::GetMouseWheelDelta(), empty_vec_i);

  ye::InputState::BeginFrame();
  EXPECT_NE(ye::InputState::GetMousePosition(), empty_vec_u);
  EXPECT_EQ(ye::InputState::GetMousePositionDelta(), empty_vec_i);
  EXPECT_EQ(ye::InputState::GetMouseWheelDelta(), empty_vec_i);
}
