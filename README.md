# Rusty C++

Personal handy sugar from rust

## Compatibility
- Latest C++ standard recommended
- Some compilers with `-std=c++11` may work

## Usage
- Simply include `rusty.hpp`, merged edition recommended.
- Define `RUSTY_NO_USE_DEFAULT` before including if you want to pick up features manually.
- Addtional `.clang-tidy` for rusty naming convention.

## Structure
All in `namespace rusty` except macros:
```
numeric_types.hpp: namespace numeric_types (
    numeric types (wrappers of numeric types, with byte manipulation helper),
    namespace literal (user-defined literals of numeric types))

collections.hpp: namespace collections (
    namespace ostream (ostream pretty printing for collections))

macros.hpp (no system header included): dbg, assert_eq, assert_ne

rusty.hpp: include those headers, add default using

numeric_types_legacy.hpp (deprecated): namespace numeric_types_legacy (
    numeric types (aliases of numeric types),
    namespace literal (user-defined literals of numeric types))
```

## More tips
- Use `auto v = Vec<u8>{1,2,3};` to replace `Vec<u8> v{1,2,3};`.
- Use `for (auto &i: v);` to replace `for (int i...);`.
- Use `const` when declaring variable as long as there is no need to modify it.
- Use smart pointers instead of raw pointers.
- Use `enum class` instead of c enum constant.

## TODO
- comments for hover doc
- more test cases
- complete arithmetic operators of numeric types