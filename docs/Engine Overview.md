# Overview
The end goal is to build a code-based game engine capable of rendering both 3D and 2D objects. No editor or scripting exists, building games is done through modules interfaces.

Each module's ultimate goal is to operate independently with minimum dependency on other modules, although this goal is naturally impractical for many modules, such as the editor and the renderer.

Each module should be replaceable by modifying its own implementation, and well-defined integration points that connect modules together into a coherent system.

Platform-specific implementations should be unified with as little overhead and complexity as possible.

Every dependency (including data structures library, logging library, macros, utility) should be explicitly justified.

The design prioritize performance in hot code paths, simplicity in platform-specific implementations, and modularity wherever applicable.
## Resource and Object Creation
In my past experiences, i have used two main patterns: singletons and factory methods. Apparently however, over-using either of them introduces additional complexity that can be avoided using a different approach.

Main systems (including `DisplayServer`, `RendererServer`, `Logger`, etc...) use the "Singleton" approach, to ensure a valid initialization state, a static "Create" method is provided:
```
static System* s_singleton;
static System* GetSingleton();
static std::expected<System*, Error> Create(...); // if successful it initializes the singleton and returns it, otherwise it returns an error.

void shutdown(); // called to clear the singleton and shutdown the system
```
Additionally, System::Create() is the owner of the singleton:
```
static System system;
...
s_singleton = &system;
```

Each system's internal resources are accessed via a `ResourceID`, and can return a handle when requested. A `ResourceID` is a class with an integer identifying resources uniquely and serves as a logical reference to the resource. Resource handles, on the other hand, are pointers to the actual resource internal data, but do not prevent resource destruction once no `ResourceIDs` reference the resource.

Modules that are not expected to fail provide a default constructor and a "Create" method:
```
Module() = default;
Error Create(...);
```

Finally, resources that can fail have a factory creation method with copy operations deleted, and explicitly defined move operation if needed.
```
static std::expected<Resource*, Error> Create(...);
Resource(const Resource&) = delete;
Resource& operator=(const Resource&) = delete;
```

## Standard Library
A custom cache-friendly memory allocator is not on the agenda for now, although it is not out of the question. I currently have two viable options: using the C++ Standard Library, or `EASTL`.

Both options are interchangeable and can be replaced later with the same amount of effort. `EASTL` is optimized for performance, whereas the C++ Standard Library is designed for general-purpose use. Using the standard library reduces external dependencies, so it is what i will use initially.

## Implementation Plan
The initial plan is to implement each module in order of increasing dependency.

The planned modules include (subject to change):
- `Logger`.
- `DisplayServer` (display management).
- `Signal/Slot` (synchronous event dispatch).
- `RendererServer`.
- `SceneServer` (high-level 2D canvas and 3D scene management).
- `ResourceServer` (CPU-side resource management).

Shared utilities and components will be developed incrementally alongside the implementation of modules.

## Build System
Among the available options, i have settled on using `CMake` for its simplicity and broad compatibility with third-party dependencies.

For now, focusing on expanding the engine codebase seems more valuable than investing time on a more complex build system. If the project requirements change in the future, i will reevaluate whether investing in a more complex build system is worthwhile.

## Coding Conventions
Formatting and naming conventions:
- Camel Case: classes, enums, functions.
- snake_case: variables.
- `m_` prefix for private member variables.
- `s_` prefix for static variables.
- `g_` prefix for global variables.
- `p` prefix for raw pointers variables (e.g. `ptexture`).
- `YE_` prefix for macros.
- `e` prefix for enums (e.g. `eError`).

Dependency rules:
- Dependencies should be completely justified.
- Dependencies are added as Git submodules and built with the project.
- Platform dependencies should never leak outside of low-level systems unless justified.

General conventions:
- Frequently used, heavy headers should belong in the precompiled header.
- Inheritance and virtual should be minimized wherever possible.
- No exceptions should be used in the engine code.
- Engine termination should be limited to high-level modules.
- General namespace name for modules is `ye`.

## Multi-Thread and Job System
There is currently no plans to implement a job system and multi-threaded execution. However, the architecture should avoid decisions that would make future multithreading unnecessarily difficult.

## Testing
All kind of automatic tests are currently deferred until the core engine architecture has stabilized.

## Build Modes
- **Release:** Maximum optimization, minimal logging, assertions disabled, debug information disabled.
- **Debug:** Optimizations disabled, maximum logging, assertions enabled, debug information enabled.
- **RelWithDebInfo:** Optimizations enabled, logging enabled, assertions disabled, debug information enabled.