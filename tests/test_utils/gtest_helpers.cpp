#include <gtest/gtest.h>
#include <core/vec2.hpp>

namespace ye {
template <typename T>
void PrintTo(const vec2<T>& v, std::ostream* os) {
  *os << '(' << v.x << ", " << v.y << ')';
}
}  // namespace ye
