#pragma once

#include <concepts>

template <typename _NumT>
  requires std::integral<_NumT> || std::floating_point<_NumT>
struct vec2 {
  _NumT x = 0;
  _NumT y = 0;

  auto operator<=>(const vec2<_NumT>&) const = default;
};
