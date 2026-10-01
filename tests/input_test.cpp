#include <gtest/gtest.h>
#include "core/input_state.hpp"
#include "test_utils/dummy_display.hpp"

using namespace ye;

TEST(Input, CorrectInitialInputState) {
  EXPECT_EQ(InputState::GetMousePositionDelta(), vec2<float>(0.0f, 0.0f));
  EXPECT_EQ(InputState::GetMousePosition(), vec2<float>(0.0f, 0.0f));
  EXPECT_EQ(InputState::GetMouseWheelDelta(), vec2<int16_t>(0, 0));
  EXPECT_FALSE(InputState::IsPressed(eInputKey::AltRight));
}

TEST(Input, CorrentInputForEvents) {
  auto ds = DummyDisplayServer();

  ds.InjectKeyEvent(eInputKey::Apostrophe, true);
  EXPECT_TRUE(InputState::IsPressed(eInputKey::Apostrophe));
  ds.InjectKeyEvent(eInputKey::F10, false);
  EXPECT_FALSE(InputState::IsPressed(eInputKey::F10));

  ds.InjectMouseEvent({.position = vec2<float>(12.0f, 14.0f)});
  EXPECT_EQ(InputState::GetMousePosition(), vec2<float>(12.0f, 14.0f));
  EXPECT_NE(InputState::GetMousePositionDelta(), vec2<float>(0.0f, 0.0f));

  ds.InjectMouseEvent({.scroll_delta = vec2<int16_t>{-5, 2}});
  EXPECT_EQ(InputState::GetMouseWheelDelta(), vec2<int16_t>(-5, 2));
}

TEST(Input, CorrectMousePositionDelta) {
  auto ds = DummyDisplayServer();
  auto id = ds.CreateWindow("dummy", 0, 0, eWindowFlagBit::WINDOW_FLAG_NONE).value();

  auto prev_position = InputState::GetMousePosition();
  vec2<float> new_position{2.12f, 3.23f};
  ds.InjectMouseEvent({.position = new_position});
  EXPECT_EQ(InputState::GetMousePositionDelta(), new_position - prev_position);

  prev_position = new_position;
  new_position = vec2<float>{600.0f, -23.9f};
  ds.InjectMouseEvent({.position = new_position});
  EXPECT_EQ(InputState::GetMousePositionDelta(), new_position - prev_position);
}
