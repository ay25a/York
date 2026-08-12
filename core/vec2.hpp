#pragma once

#include <compare>
#include <concepts>

template <typename _NumT>
  requires std::integral<_NumT> || std::floating_point<_NumT>
struct vec2 {
  _NumT x = 0;
  _NumT y = 0;

  constexpr vec2(_NumT x, _NumT y) noexcept {
    this->x = x;
    this->y = y;
  }

  constexpr vec2() noexcept = default;
  constexpr vec2(const vec2&) noexcept = default;
  constexpr vec2& operator=(const vec2&) noexcept = default;

  constexpr vec2(vec2&&) noexcept = default;
  constexpr vec2& operator=(vec2&&) noexcept = default;
  constexpr ~vec2() noexcept = default;

  constexpr auto operator<=>(const vec2<_NumT>&) const noexcept = default;

  constexpr vec2 operator-(const vec2 rhs) const noexcept {
    return vec2(this->x - rhs.x, this->y - rhs.y);
  }

  constexpr vec2 operator+(const vec2 rhs) const noexcept {
    return vec2(this->x + rhs.x, this->y + rhs.y);
  }

  constexpr vec2& operator-=(const vec2& rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
  }

  constexpr vec2<_NumT>& operator+=(const vec2& rhs) noexcept {
    x += rhs.x;
    y += rhs.y;

    return *this;
  }
};
