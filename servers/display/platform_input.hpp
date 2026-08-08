#pragma once

#include <cstdint>
#include <unordered_map>
#include "core/assertion.hpp"
#include "core/input_enum.hpp"

namespace ye {
class PlatformInput {
  friend class DisplayServer;

 private:
  static std::unordered_map<uint32_t, eInputKey> s_key_map;
  virtual void Initialize() noexcept = 0;

 public:
  static eInputKey TranslateKey(uint32_t native_key) noexcept {
    YE_ASSERT(s_key_map.contains(native_key), "Platform input has not been initialized!");
    return s_key_map[native_key];
  }
};
}  // namespace ye
