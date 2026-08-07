## Input
Keyboard keys and mouse buttons are represented using engine-defined enum, independent of platform-specific key codes.
## Input State
`InputState` stores the latest state of input devices, including keyboard keys, mouse buttons, and mouse cursor position (window-local coordinates).

Its state is updated continuously, and can be queried independently of the `DisplayServer` and window events.

`InputState` is a complete static class which contains:
- `bitset<KEY_CODE_MAX>` representing the current state of both keyboard keys and mouse buttons.
- `vec2<uint16_t>` representing the current position of the mouse (window-local coordinates).
## `DisplayServer` Relation
`DisplayServer` initializes the translation layer between platform-specific input codes and the engine enums.

It is also responsible for updating the global `InputState` independently of window-specific `EventManager`.

## Implementation Details
- [ ] Design `InputState` and `Input` diagrams.
- [ ] Implement `InputState` and `Input`.
- [ ] Design unit tests for `InputState`.
- [ ] Implement `InputState` unit testing.