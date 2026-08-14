#include "wayland_display_server.hpp"

namespace ye {
eError WaylandDisplayServer::Initialize() noexcept {
  if (m_display = wl_display_connect(nullptr); !m_display)
    return ERR_CANNOT_CREATE;

  if (m_registry = wl_display_get_registry(m_display); !m_registry)
    return ERR_CANNOT_CREATE;

  wl_registry_add_listener(m_registry, &REGISTERY_LISTENER, static_cast<void*>(this));
  while (!(m_output && m_compositor && m_wm_base && m_wlroots_shell))
    wl_display_roundtrip(m_display);

  xdg_wm_base_add_listener(m_wm_base, &WM_BASE_LISTENER, nullptr);
  return SUCCESS;
}

std::expected<void*, eError> WaylandDisplayServer::PlatformCreateWindow(const WindowCreateInfo& ci) noexcept {
  wl_surface* surface = wl_compositor_create_surface(m_compositor);
  if (!surface)
    return std::unexpected(ERR_CANNOT_CREATE);

  wl_surface_commit(surface);
  return static_cast<void*>(surface);
}

void WaylandDisplayServer::PlatformDestroyWindow(void* handle) noexcept {
  wl_surface_destroy(static_cast<wl_surface*>(handle));
}

void WaylandDisplayServer::registery_add(void* data, wl_registry* reg, uint32_t name, const char* interface, uint32_t version) {
  static_cast<WaylandDisplayServer*>(data)->HandleRegistery(name, interface, version);
}

void WaylandDisplayServer::wm_base_ping(void* data, xdg_wm_base* base, uint32_t serial) {
  xdg_wm_base_pong(base, serial);
}

void WaylandDisplayServer::HandleRegistery(uint32_t name, std::string_view interface, uint32_t version) {
  if (interface == wl_compositor_interface.name)
    m_compositor = static_cast<wl_compositor*>(wl_registry_bind(m_registry, name, &wl_compositor_interface, version));
  else if (interface == xdg_wm_base_interface.name)
    m_wm_base = static_cast<xdg_wm_base*>(wl_registry_bind(m_registry, name, &xdg_wm_base_interface, version));
  else if (interface == wl_output_interface.name)
    m_output = static_cast<wl_output*>(wl_registry_bind(m_registry, name, &wl_output_interface, version));
  else if (std::string_view(interface) == zwlr_layer_shell_v1_interface.name)
    m_wlroots_shell = static_cast<zwlr_layer_shell_v1*>(wl_registry_bind(m_registry, name, &zwlr_layer_shell_v1_interface, version));
}
}  // namespace ye
