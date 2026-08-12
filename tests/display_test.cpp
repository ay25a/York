#include <gtest/gtest.h>
#include "test_utils/dummy_display.hpp"

using namespace ye;

TEST(DisplayTest, WindowIDGeneration) {
  {
    DummyDisplay ds;

    ASSERT_TRUE(ds.GetServer() != nullptr);

    EXPECT_EQ(ds.GetServer()->GetWindowCount(), 0);
    EXPECT_EQ(ds.GetServer()->CreateWindow({}).value().Get(), 0);
    EXPECT_EQ(ds.GetServer()->GetWindowCount(), 1);

    WindowID created = ds.GetServer()->CreateWindow({}).value();
    EXPECT_EQ(ds.GetServer()->GetWindowCount(), 2);
    ds.GetServer()->DestroyWindow(created);
    EXPECT_EQ(ds.GetServer()->GetWindowCount(), 1);
  }
  {
    DummyDisplay ds;
    EXPECT_EQ(ds.GetServer()->GetWindowCount(), 0);
    EXPECT_EQ(ds.GetServer()->CreateWindow({}).value().Get(), 0);
    EXPECT_EQ(ds.GetServer()->GetWindowCount(), 1);
  }
}

TEST(DisplayTest, CorrectWindowCreation) {
  DummyDisplay ds;

  DisplayServer::WindowCreateInfo ci{
      .title = "Hello",
      .mode = eWindowMode::Fullscreen,
      .width = 680,
      .height = 680,
      .flags = eWindowFlagBit::WINDOW_FLAG_NO_RESIZE_BIT,
  };

  WindowID id = DisplayServer::GetSingleton().CreateWindow(ci).value();
  Window& win = DisplayServer::GetSingleton().GetWindow(id);

  EXPECT_EQ(win.GetHandle(), nullptr);
  EXPECT_EQ(win.GetMode(), ci.mode);
  EXPECT_TRUE(win.GetSize() == vec2<uint16_t>(ci.width, ci.height));
  EXPECT_EQ(win.GetTitle(), ci.title);
  EXPECT_TRUE(win.IsFocused());
  EXPECT_TRUE(win.HasFlag(eWindowFlagBit::WINDOW_FLAG_NO_RESIZE_BIT));
  EXPECT_FALSE(win.HasFlag(eWindowFlagBit::WINDOW_FLAG_MAX_BIT));
}
