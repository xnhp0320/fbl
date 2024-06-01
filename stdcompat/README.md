This library provides polyfills for different versions of the standard library.

Constructs are withing namespaces for each specific version, such as "cpp17, cpp20".

Polyfills will only be used when the standard version is not available.

In some cases, some free functions may be provided. These functions fill gaps of classes
that are extended in later iterations of the standard (e.g. std::string_view::starts_with).
In the above example, std::string_view polyfill would live in cpp17::string_view and a free
function should exist in cpp20::starts_with(std::string_view, prefix).

The library has a set of macros for testing or enforcing different conditions. All that is required is for this symbols to be defined.

- LIB_STDCOMPAT_USE_POLYFILLS : Will pick polyfill implementation over std.

- LIB_STDCOMPAT_NO_INLINE_VARIABLES: Uses stdcompat alternative for inline variables.