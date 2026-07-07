## Error handling and Reporting
The engine uses 2 mechanisms for error handling:
- Using `std::expected`.
- Using an error enum (`eError`).

The `std::expected` approach is primarily intended for lightweight returning types, such as pointers, handles, small trivially copyable objects.

Functions that don't return a success value should return `eError`. Functions that must also return a lightweight return value should return `std::expected<T, eError>`.

Expensive-to-copy objects should generally not be returned by value. Instead, objects should be accessed through resource IDs, references, smart pointers, or any other ownership model. Where returning by value cannot be avoided, move semantics should be preferred.

## Error enum
`eError` is a C-style enum used mainly for recoverable error handling. Error names should be written in uppercase and prefixed with `ERR_`. However, the first error value should be named `Ok` with a value equal to 0.

Error codes should be general-purpose and only become more specified when necessary. Additionally, they should also added incrementally during the development process.

`eError` enum should provide string conversion function and an `std::formatter` specialization for integration with `std::format`.

## Assertion
Assertions should be enabled in both `Debug` and `RelWithDebInfo` modes, and disabled in `Release` mode. Each assertion should include a descriptive error failure message.

Assertions should be avoided in hot code paths. They should only be used to indicate programmer errors or invalid assumptions indicating a bug in the engine
