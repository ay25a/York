#pragma once

#include <cstdint>
#include <unordered_map>
#include "core/input.hpp"

namespace ye {
class PlatformInput {
  friend class DisplayServer;

 private:
  std::unordered_map<uint32_t, eInputKey> m_key_map;

  virtual void InitInputMap() = 0;
  inline eInputKey TranslateKey(uint32_t native_key) const { return m_key_map[native_key]; }
};
}  // namespace ye
