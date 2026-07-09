 ## External Logging Library
`spdlog` is used as a logging library due to its familiarity and performance.

## Specifications
There are 5 logging levels:
- Trace: detailed diagnostic messages used mainly to trace execution flow and assist in debugging.
- Info: Significant events during the application execution.
- Warning: Unexpected behavior or potential misuse that does not prevent execution.
- Error: Recoverable errors that should be addressed.
- Fatal/Critical: An unrecoverable error that requires application termination. Any queued trace messages are flushed before shutdown.

Logging output may be directed to console, a file, or both simultaneously.

## Build Type Behavior
#### `Debug`: 
- Console logger enabled.
- File logger available but disabled by default at runtime.
- All logging levels enabled.
- Trace messages are printed immediately by default, but may optionally be queued.

#### `RelWithDebInfo`:
- Console logger enabled.
- File logger available but disabled by default at runtime.
- Info logging disabled
- Trace messages are buffered (up to 32 entries) and flushed only after a fatal error.

#### Release:
- File logger enabled.
- Info logging disabled.
- Trace messages are buffered (up to 32 entries) and written only after a fatal error.

## General Behavior
Logging is permitted from all engine modules and avoided in utility code. Logging within performance-critical main-loop code should be kept at minimum.

Although fully synchronous logging is acceptable, asynchronous logging is used by default because some logging inside the main-loop is unavoidable.

Fatal errors should write a final log entry, flush the message queue, shutdown the logger, and terminate the application.
## Project relation
Two independent loggers are available: one for engine code and one for the client, both can be created regardless of the build type. 

## Message formatting
The general message formatting should follow "\[Logger Logging Level - Timestamp]: Message", for example:
```
[Engine error | 12:11:05]: Texture could not be loaded.
```

Fatal Error messages includes the file path, line number, and function name:
```
[Engine trace | 12:10:06]: ...
[Engine critical | 12:11:06]: Internal platform error occured at display_server.cpp:21 at CreateWindow().
```

## Macros
- `YE_LOGGER_CLIENT_MACROS`: Enable or disable client macros.
- `YE_ENGINE_TRACE`, `YE_ENGINE_INFO`, `YE_ENGINE_WARN, `YE_ENGINE_ERROR`: Engine logger shortcuts.
- `YE_CLIENT_TRACE`, `YE_CLIENT_INFO`, `YE_CLIENT_WARN`, `YE_CLIENT_ERROR`: Client logger shortcuts.

*note*: Fatal logging is intentionally not exposed through a macro; since it fatal logging must flush any buffered messages, write a fatal entry, and terminating the program.
