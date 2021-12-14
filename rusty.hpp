// SPDX-License-Identifier: MIT
// Copyright(c) 2021 ur4t

#ifndef __RUSTY_HPP__
#define __RUSTY_HPP__

#include <ostream>
#include <unordered_map>
#include <vector>

namespace rusty::collections {

template <typename T1, typename T2> using Pair = ::std::pair<T1, T2>;
template <typename T, ::std::size_t N> using Array = ::std::array<T, N>;
template <typename T> using Vec = ::std::vector<T>;
template <typename K, typename V> using HashMap = ::std::unordered_map<K, V>;

namespace ostream {

inline ::std::string prepare(const ::std::string &s) { return "\"" + s + "\""; }
inline ::std::int32_t prepare(const ::std::int8_t &t) { return t; }
inline ::std::int32_t prepare(const ::std::uint8_t &t) { return t; }
template <typename T> inline const T &prepare(const T &t) { return t; }

template <typename T1, typename T2>
::std::ostream &operator<<(::std::ostream &s, const Pair<T1, T2> &p) {
    return s << "(" << prepare(p.first) << ", " << prepare(p.second) << ")";
}

#define OSTREAM_IMPL(T1, T2)                                                   \
    template <T1>::std::ostream &operator<<(::std::ostream &s, const T2 &c) {  \
        auto count = c.size();                                                 \
        s << "[";                                                              \
        for (const auto &i : c) {                                              \
            s << prepare(i);                                                   \
            if (--count > 0) {                                                 \
                s << ", ";                                                     \
            }                                                                  \
        }                                                                      \
        return s << "]";                                                       \
    }

#define COMMA ,

OSTREAM_IMPL(typename T COMMA ::std::size_t size, Array<T COMMA size>)
OSTREAM_IMPL(typename T, Vec<T>)
OSTREAM_IMPL(typename K COMMA typename V, HashMap<K COMMA V>)

#undef COMMA
#undef OSTREAM_IMPL

} // namespace ostream

} // namespace rusty::collections

// use a temporary variable in case that @x contains side effect
#define dbg(x, ...)                                                            \
    [&] {                                                                      \
        auto t = (x);                                                          \
        auto flag = ::std::cerr.flags();                                       \
        auto precision = ::std::cerr.precision();                              \
        ::std::cerr << "[" __FILE__ ":" << __LINE__                            \
                    << "] " #x " = " __VA_ARGS__ << t << ::std::endl;          \
        ::std::cerr.flags(flag);                                               \
        ::std::cerr.precision(precision);                                      \
        return t;                                                              \
    }()

#define assert_eq(left, right) assert((left) == (right))
#define assert_ne(left, right) assert(!((left) == (right)))

#include <array>
#include <cstring>
#include <limits>
#include <ostream>

namespace rusty::numeric_types {

#if __cplusplus < 201402L
template <bool B, typename T = void> // NOLINTNEXTLINE
using enable_if_t = typename ::std::enable_if<B, T>::type;
#else
using ::std::enable_if_t;
#endif

#if __cplusplus < 201703L
template <typename T> // NOLINTNEXTLINE(readability-identifier-naming)
constexpr bool is_trivial_v = ::std::is_trivial<T>::value;
#else
using ::std::is_trivial_v;
#endif

// bit casting from https://en.cppreference.com/w/cpp/numeric/bit_cast
template <class T1, class T2>
constexpr enable_if_t<
    sizeof(T1) == sizeof(T2) && is_trivial_v<T2> && is_trivial_v<T1>, T1>
bit_cast(const T2 &from) noexcept {
    T1 to;
    ::std::memcpy(&to, &from, sizeof(T1));
    return to;
}

template <typename T> struct Number {
    template <::std::size_t N>
    using Bytes = ::std::array<Number<::std::uint8_t>, N>;
    using Self = Number<T>;
    T value;

    // NOLINTNEXTLINE(google-explicit-constructor)
    inline constexpr operator T() const & { return this->value; }
    // NOLINTNEXTLINE(google-explicit-constructor)
    inline constexpr Number<T>(const T &value) : value(value){};
    inline constexpr Number<T>() = default;
    inline constexpr Number<T>(const Self &) = default;

    // compile time endian detection from
    // https://en.cppreference.com/w/cpp/types/endian
#ifdef _WIN32
    static constexpr bool is_le = true;  // NOLINT
    static constexpr bool is_be = false; // NOLINT
#else
    static constexpr bool is_le = // NOLINT
        __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;
    static constexpr bool is_be = // NOLINT
        __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__;
#endif
    static_assert(is_le || is_be, "Neither little-endian or big-endian!");

    static constexpr Self from_ne_bytes(const Bytes<sizeof(T)> &bytes) {
        return bit_cast<Self>(bytes);
    }

    constexpr Bytes<sizeof(T)> to_ne_bytes() const & {
        return bit_cast<Bytes<sizeof(T)>>(value);
    }

    // Based on https://en.cppreference.com/w/cpp/container/array/rbegin
    template <::std::size_t N>
    static constexpr Bytes<N> reverse_bytes(Bytes<N> bytes) noexcept {
        Bytes<N> reversed_bytes;
        ::std::copy(bytes.crbegin(), bytes.crend(), reversed_bytes.begin());
        return reversed_bytes;
    }

    constexpr Self reverse_bytes() const & {
        return from_ne_bytes(reverse_bytes(to_ne_bytes()));
    }

#define BYTE_CONVERT_IMPL(e)                                                   \
    static constexpr Self from_##e##_bytes(const Bytes<sizeof(T)> &bytes) {    \
        return from_ne_bytes(is_##e ? bytes : reverse_bytes(bytes));           \
    }                                                                          \
    static constexpr Self from_##e(const Self &source) {                       \
        return is_##e ? source : source.reverse_bytes();                       \
    }                                                                          \
    constexpr Bytes<sizeof(T)> to_##e##_bytes() const & {                      \
        return is_##e ? to_ne_bytes() : reverse_bytes(to_ne_bytes());          \
    }                                                                          \
    constexpr Self to_##e() const & {                                          \
        return is_##e ? Self{value} : reverse_bytes();                         \
    }

    BYTE_CONVERT_IMPL(le)
    BYTE_CONVERT_IMPL(be)

#undef BYTE_CONVERT_IMPL

#define INC_DEC_IMPL(op, pos, pre, post)                                       \
    inline Self operator op(pos) & { return Self{pre value post}; }

    INC_DEC_IMPL(++, , ++, )
    INC_DEC_IMPL(--, , --, )
    INC_DEC_IMPL(++, int, , ++) // NOLINT(cert-dcl21-cpp)
    INC_DEC_IMPL(--, int, , ++) // NOLINT(cert-dcl21-cpp)

#undef INC_DEC_IMPL

#define BINARY_ASSIGN_IMPL(op)                                                 \
    inline Self operator op##=(const Self &oprand) & {                         \
        return Self{value op## = oprand.value};                                \
    }

    BINARY_ASSIGN_IMPL(+)
    BINARY_ASSIGN_IMPL(-)
    BINARY_ASSIGN_IMPL(*)
    BINARY_ASSIGN_IMPL(/)
    BINARY_ASSIGN_IMPL(%)
    BINARY_ASSIGN_IMPL(&)
    BINARY_ASSIGN_IMPL(|)
    BINARY_ASSIGN_IMPL(^)
    BINARY_ASSIGN_IMPL(<<)
    BINARY_ASSIGN_IMPL(>>)

#undef BINARY_ASSIGN_IMPL

    friend ::std::ostream &operator<<(::std::ostream &s, const Self &number) {
        return s << +number.value;
    }

    friend ::std::istream &operator>>(::std::istream &s, Self &number) {
        return s >> number.value;
    }
};

// TODO(i128/u128)

#define GENERAL_INT_IMPL(ALIAS, ORIGIN)                                        \
    using ALIAS = Number<ORIGIN>;                                              \
    namespace literal {                                                        \
    inline constexpr ALIAS operator""_##ALIAS(unsigned long long int val) {    \
        return static_cast<ALIAS>(val);                                        \
    }                                                                          \
    } // namespace literal

#define FIXED_INT_IMPL(BITS)                                                   \
    GENERAL_INT_IMPL(i##BITS, int##BITS##_t)                                   \
    GENERAL_INT_IMPL(u##BITS, uint##BITS##_t)

#define FLOATING_IMPL(BITS, ORIGIN)                                            \
    using f##BITS = Number<ORIGIN>;                                            \
    static_assert(::std::numeric_limits<ORIGIN>::is_iec559 &&                  \
                      sizeof(ORIGIN) == (BITS) / 8,                            \
                  "f" #BITS " illegal!");                                      \
    namespace literal {                                                        \
    inline constexpr f##BITS operator""_f##BITS(long double val) {             \
        return static_cast<f##BITS>(val);                                      \
    }                                                                          \
    } // namespace literal

FIXED_INT_IMPL(8)
FIXED_INT_IMPL(16)
FIXED_INT_IMPL(32)
FIXED_INT_IMPL(64)

GENERAL_INT_IMPL(isize, intptr_t)
GENERAL_INT_IMPL(usize, uintptr_t)

FLOATING_IMPL(32, float)
FLOATING_IMPL(64, double)

#undef FIXED_INT_IMPL
#undef FLOATING_IMPL
#undef GENERAL_INT_IMPL

} // namespace rusty::numeric_types

template <typename T> struct std::hash<::rusty::numeric_types::Number<T>> {
    ::std::size_t constexpr operator()(
        const ::rusty::numeric_types::Number<T> &number) const &noexcept {
        return ::std::hash<T>{}(number.value);
    }
};

#ifdef RUSTY_USE_DEFAULT_USING

using namespace ::rusty::numeric_types;
using namespace ::rusty::numeric_types::literal;
using namespace ::rusty::collections;
using namespace ::rusty::collections::ostream;

#endif

#endif // __RUSTY_HPP__
