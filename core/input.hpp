#pragma once

namespace ye {
// clang-format off
enum class eInputKey {
  A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
  N0, N1, N2, N3, N4, N5, N6, N7, N8, N9,
  F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
  ArrowLeft, ArrowRight, ArrowUp, ArrowDown,
  MouseLeft, MouseRight, MouseMiddle, MouseX1, MouseX2,

  ControlLeft, ControlRight,
  ShiftLeft, ShiftRight,
  AltLeft, AltRight,
  Super,

  CapsLock, NumLock, ScrollLock,

  Space,
  Enter,
  Tab,
  Backspace,
  Escape,
  Insert,
  Delete,
  Home,
  End,
  PageUp,
  PageDown,

  GraveAccent,   // `
  Minus,         // -
  Equal,         // =
  LeftBracket,   // [
  RightBracket,  // ]
  Backslash,     // '\'
  Semicolon,     // ;
  Apostrophe,    // '
  Comma,         // ,
  Period,        // .
  Slash,         // /

  Count
};
// clang-format on
}  // namespace ye
