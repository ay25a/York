find_program(WAYLAND_SCANNER NAMES wayland-scanner REQUIRED)

macro(wayland_append_protocol PROTOCOL_NAME)
  set(XML_PATH "${CMAKE_CURRENT_LIST_DIR}/protocols/${PROTOCOL_NAME}.xml")
  set(PROTOCOL_HEADER "${CMAKE_CURRENT_LIST_DIR}/protocols/${PROTOCOL_NAME}-client-protocol.h")
  set(PROTOCOL_SOURCE "${CMAKE_CURRENT_LIST_DIR}/protocols/${PROTOCOL_NAME}-protocol.cpp")

  execute_process(
    COMMAND "${WAYLAND_SCANNER}" client-header "${XML_PATH}" "${PROTOCOL_HEADER}"
    COMMAND_ERROR_IS_FATAL ANY
  )

  execute_process(
    COMMAND "${WAYLAND_SCANNER}" private-code "${XML_PATH}" "${PROTOCOL_SOURCE}"
    COMMAND_ERROR_IS_FATAL ANY
  )

  list(APPEND YORK_HEADERS "${PROTOCOL_HEADER}")
  list(APPEND YORK_SOURCES "${PROTOCOL_SOURCE}")
endmacro()

wayland_append_protocol("xdg_shell")
wayland_append_protocol("wlr-layer-shell-unstable-v1")

list(APPEND YORK_SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/wayland_display_server.cpp"
)

list(APPEND YORK_HEADERS
  "${CMAKE_CURRENT_LIST_DIR}/wayland_display_server.hpp"
)
