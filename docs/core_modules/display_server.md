## Display Server
`DisplayServer` is a platform-specific abstract class that is specialized for each supported platform.

It manages window creation and destruction, process native platform events, dispatches window events, and implement platform-specific windowing logic.
## Event Manager
`EventManager` is owned by `Window`. It managed event subscriptions, and dispatches window events to subscribers.

Managed events include:
- Window state changes (full screen, maximized, minimized).
- Window focus changes.
- Window size changes.
- Keyboard key presses and releases.
- Mouse button presses and releases.
- Mouse wheel events.
- Mouse position changes (window local coordinates).

Events are generated for individual windows, and only the latest event of each type is stored except for key presses and releases.

In the case of multiple keyboard and mouse key events per frame, key presses are combined through a bit-mask approach, while key releases remove the pressed key if exist.

Unsubscribing from window events can be manual, or automatic when the subscriber is destroyed.

Event storage should prioritize cache utilization since event processing may occur each frame.

**NOTE: Input is additionally processed into a global input state by the `DisplayServer`**
## Window
`Window` is platform-independent designed as a container for its ID, state, size, and event manager.

`DisplayServer` owns all `Window` objects. Other systems should access windows only through IDs.

Only one *main window* exists with a fixed ID. Multiple *sub-windows* may exist. Destroying the main window destroys all remaining windows.
## Input
Input and its related utilities and enum exist in the [[input_state]] module.

`DisplayServer` initializes the translation from native platform input into the unified engine input enum variation. It is also responsible for updating the `InputState` independent of `window` input handling. 

## Implementation Steps
- [ ] prerequisite: Finish [[input_state]] implementation.
- [ ] Design `DisplayServer` and `Window` diagrams.
- [ ] Design `WaylandDisplayServer` diagram.
- [ ] Implement `DisplayServer`, `Window`, and `WaylandDisplayServer`.
- [ ] Update [[display_server]] documentation and diagrams if any.
- [ ] Design unit tests for `DisplayServer`.
- [ ] Implement unit tests for `DisplayServer`.