#include <gtest/gtest.h>
#include "test_utils/dummy_display.hpp"

using namespace ye;

TEST(DisplayTest, WindowIDGeneration) {
  while (0) {
    DummyDisplay ds;

    EXPECT_EQ(ds.GetServer().GetWindowCount(), 0);
    EXPECT_EQ(ds.GetServer().CreateWindow({}).value().get(), 0);
    EXPECT_EQ(ds.GetServer().GetWindowCount(), 1);

    WindowID created = ds.GetServer().CreateWindow({}).value();
    EXPECT_EQ(ds.GetServer().GetWindowCount(), 2);
    ds.GetServer().DestroyWindow(created);
    EXPECT_EQ(ds.GetServer().GetWindowCount(), 1);
  }

  DummyDisplay ds;
  EXPECT_EQ(ds.GetServer().GetWindowCount(), 0);
  EXPECT_EQ(ds.GetServer().CreateWindow({}).value().get(), 0);
  EXPECT_EQ(ds.GetServer().GetWindowCount(), 1);
}

TEST(DisplayTest, CorrectWindowCreation) {
  DummyDisplay ds;

  DisplayServer::WindowCreateInfo ci{
      .title = "Hello",
      .mode = eWindowMode::Fullscreen,
      .width = 680,
      .height = 680,
  };

  WindowID id = ds.GetServer().CreateWindow(ci).value();
  const Window& win = ds.GetServer().GetWindow(id);

  EXPECT_EQ(win.mode, ci.mode);
  EXPECT_EQ(win.size, vec2<uint16_t>(ci.width, ci.height));
  EXPECT_EQ(win.title, ci.title);
}

TEST(DisplayTest, CorrectWindowEventHandling) {
  DummyDisplay ds;

  DisplayServer::WindowCreateInfo ci{
      .mode = eWindowMode::Fullscreen,
      .width = 640,
      .height = 540,
  };

  auto id = ds.GetServer().CreateWindow(ci).value();

  ds.GetServer().InjectWindowEvent(id, {.mode = eWindowMode::Minimized});
  EXPECT_EQ(ds.GetServer().GetWindow(id).mode, eWindowMode::Minimized);

  ds.GetServer().InjectWindowEvent(id, {.size = vec2<uint16_t>(640, 540)});
  EXPECT_EQ(ds.GetServer().GetWindow(id).size, vec2<uint16_t>(640, 540));

  EXPECT_FALSE(ds.GetServer().IsWindowFocused(id));
  ds.GetServer().InjectWindowEvent(id, {.switch_focus = true});
  EXPECT_TRUE(ds.GetServer().IsWindowFocused(id));
}
