#pragma once

#include <compare>
#include <cstdint>
#include <functional>

namespace ye {
struct WindowID {
  friend class DisplayServer;

  constexpr WindowID() noexcept = default;
  constexpr auto operator<=>(const WindowID&) const noexcept = default;
  constexpr int32_t get() const noexcept { return id; }

 private:
  constexpr WindowID& operator++() noexcept {
    id++;
    return *this;
  }

  constexpr WindowID& operator--() noexcept {
    id--;
    return *this;
  }

 private:
  int32_t id = -1;
};
}  // namespace ye

namespace std {
template <>
struct hash<ye::WindowID> {
  size_t operator()(const ye::WindowID& wid) const noexcept {
    return std::hash<decltype(wid.get())>{}(wid.get());
  }
};
};  // namespace std
