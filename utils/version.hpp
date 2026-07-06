#pragma once

#include <cstdint>
#include <string>
#include <format>

struct Version {
 private:
  constexpr Version(uint32_t raw) : m_version(raw) {}

 public:
  Version() = default;
  ~Version() = default;

  static constexpr Version make(uint16_t major, uint16_t minor, uint16_t patch) {
    return Version((static_cast<uint32_t>(major) << 22) |
                   (static_cast<uint32_t>(minor) << 12) |
                   (static_cast<uint32_t>(patch)));
  }

  constexpr uint16_t major() const noexcept { return (m_version >> 22) & 0x3FF; }
  constexpr uint16_t minor() const noexcept { return (m_version >> 12) & 0x3FF; }
  constexpr uint16_t patch() const noexcept { return m_version & 0xFFF; }

  constexpr uint32_t raw() const noexcept { return m_version; }
  constexpr std::string text() const { return std::format("{}.{}.{}", major(), minor(), patch()); }

  constexpr auto operator<=>(const Version&) const noexcept = default;

 private:
  uint32_t m_version = 0;
};
