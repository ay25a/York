#pragma once

#include "servers/display_server.hpp"
#include <wayland-client.h>
extern "C" {
#include "protocols/xdg_shell-client-protocol.h"
#include "protocols/wlr-layer-shell-unstable-v1-client-protocol.h"
}

namespace ye {

class WaylandDisplayServer : DisplayServer {
 private:
  std::unordered_map<WindowID, zwlr_layer_shell_v1_layer*> m_layers;

 private:
  virtual ~WaylandDisplayServer() noexcept;
  eError Initialize() noexcept override final;

  std::expected<void*, eError> PlatformCreateWindow(const WindowCreateInfo& ci) noexcept override final;
  void PlatformDestroyWindow(void* handle) noexcept override final;
  void CreateInputMap() noexcept override;
  void PlatformPollEvents() const noexcept override;

 private:
  virtual void HandleRegistery(uint32_t name, std::string_view interface, uint32_t version);
  static void registery_add(void* data, wl_registry* reg, uint32_t name, const char* interface, uint32_t version);
  static void register_remove(void* data, wl_registry* reg, uint32_t name);

  static void wm_base_ping(void* data, xdg_wm_base* base, uint32_t serial);

 public:
  static constexpr wl_registry_listener REGISTERY_LISTENER = {&registery_add, &register_remove};
  static constexpr xdg_wm_base_listener WM_BASE_LISTENER = {&wm_base_ping};

 private:
  wl_display* m_display = nullptr;
  wl_registry* m_registry = nullptr;
  wl_compositor* m_compositor = nullptr;
  wl_output* m_output = nullptr;
  xdg_wm_base* m_wm_base = nullptr;
  zwlr_layer_shell_v1* m_wlroots_shell = nullptr;
};

}  // namespace ye
