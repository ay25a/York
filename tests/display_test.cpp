#include <gtest/gtest.h>
#include "test_utils/dummy_display.hpp"

using namespace ye;

TEST(DisplayTest, WindowIDGeneration) {
  while (0) {
    auto ds = DummyDisplayServer();

    EXPECT_EQ(ds.GetWindowCount(), 0);
    EXPECT_EQ(ds.CreateWindow("", 0, 0, ye::WINDOW_FLAG_NONE).value(), 0);
    EXPECT_EQ(ds.GetWindowCount(), 1);

    WindowID created = ds.CreateWindow("", 0, 0, ye::WINDOW_FLAG_NONE).value();
    EXPECT_EQ(ds.GetWindowCount(), 2);
    ds.DestroyWindow(created);
    EXPECT_EQ(ds.GetWindowCount(), 1);
  }

  auto ds = DummyDisplayServer();
  EXPECT_EQ(ds.GetWindowCount(), 0);
  EXPECT_EQ(ds.CreateWindow("", 0, 0, ye::WINDOW_FLAG_NONE).value(), 0);
  EXPECT_EQ(ds.GetWindowCount(), 1);
}

TEST(DisplayTest, CorrectWindowCreation) {
  auto ds = DummyDisplayServer();

  const char* title = "Dummy";
  uint16_t width = 688;
  uint16_t height = 854;

  WindowID id = ds.CreateWindow(title, width, height, ye::WINDOW_FLAG_NONE).value();
  const Window& win = ds.GetWindow(id);

  EXPECT_EQ(win.mode, eWindowMode::Windowed);
  EXPECT_EQ(win.size, vec2<uint16_t>(width, height));
  EXPECT_EQ(win.title, title);
}

TEST(DisplayTest, CorrectWindowEventHandling) {
  auto ds = DummyDisplayServer();

  auto id = ds.CreateWindow("", 640, 540, ye::WINDOW_FLAG_NONE).value();

  ds.InjectWindowEvent(id, {.mode = eWindowMode::Minimized});
  EXPECT_EQ(ds.GetWindow(id).mode, eWindowMode::Minimized);

  ds.InjectWindowEvent(id, {.size = vec2<uint16_t>(640, 540)});
  EXPECT_EQ(ds.GetWindow(id).size, vec2<uint16_t>(640, 540));

  EXPECT_TRUE(ds.IsWindowFocused(id));
  ds.InjectWindowEvent(id, {.focus = false});
  EXPECT_FALSE(ds.IsWindowFocused(id));
}
