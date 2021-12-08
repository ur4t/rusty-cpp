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

#define OSTREAM_IMPL(s, c)                                                     \
    auto count = (c).size();                                                   \
    (s) << "[";                                                                \
    for (const auto &i : (c)) {                                                \
        (s) << prepare(i);                                                     \
        if (--count > 0) {                                                     \
            (s) << ", ";                                                       \
        }                                                                      \
    }                                                                          \
    return (s) << "]";

template <typename T, ::std::size_t size>
::std::ostream &operator<<(::std::ostream &s, const Array<T, size> &a) {
    OSTREAM_IMPL(s, a);
}

template <typename T>
::std::ostream &operator<<(::std::ostream &s, const Vec<T> &v) {
    OSTREAM_IMPL(s, v);
}

template <typename K, typename V>
::std::ostream &operator<<(::std::ostream &s, const HashMap<K, V> &m) {
    OSTREAM_IMPL(s, m);
}

#undef OSTREAM_IMPL

} // namespace ostream

} // namespace rusty::collections

#define dbg(x, ...)                                                            \
    [&] {                                                                      \
        auto flag = ::std::cerr.flags();                                       \
        auto precision = ::std::cerr.precision();                              \
        ::std::cerr << "[" __FILE__ ":" << __LINE__                            \
                    << "] " #x " = " __VA_ARGS__ << (x) << ::std::endl;        \
        ::std::cerr.flags(flag);                                               \
        ::std::cerr.precision(precision);                                      \
        return x;                                                              \
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
template <class To, class From>
constexpr enable_if_t<
    sizeof(To) == sizeof(From) && is_trivial_v<From> && is_trivial_v<To>, To>
bit_cast(const From &source) noexcept {
    To destination;
    ::std::memcpy(&destination, &source, sizeof(To));
    return destination;
}

template <typename T> struct Number {

    template <::std::size_t N>
    using ByteArray = ::std::array<Number<::std::uint8_t>, N>;
    using Self = Number<T>;

    T value;

    constexpr Number<T>() = default;
    constexpr Number<T>(const Self &permitive) = default;

    // NOLINTNEXTLINE(google-explicit-constructor)
    constexpr Number<T>(const T &value) : value(value){};

    // byte convert begin =====================================================

    // compile time endian detection from
    // https://en.cppreference.com/w/cpp/types/endian
#ifdef _WIN32
    static constexpr bool IS_LE = true;
    static constexpr bool IS_BE = false;
#else
    static constexpr bool IS_LE = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;
    static constexpr bool IS_BE = __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__;
#endif
    static_assert(IS_LE || IS_BE, "Neither little-endian or big-endian!");

    static constexpr Self from_ne_bytes(const ByteArray<sizeof(T)> &bytes) {
        return Self{bit_cast<T>(bytes)};
    }

    constexpr ByteArray<sizeof(T)> to_ne_bytes() const {
        return bit_cast<ByteArray<sizeof(T)>>(value);
    }

    // Based on https://en.cppreference.com/w/cpp/container/array/rbegin
    template <::std::size_t N>
    static constexpr ByteArray<N> reverse_bytes(ByteArray<N> bytes) {
        ByteArray<N> reversed_bytes;
        ::std::copy(bytes.crbegin(), bytes.crend(), reversed_bytes.begin());
        return reversed_bytes;
    }

    constexpr Self reverse_bytes() const {
        return Self::from_ne_bytes(reverse_bytes(this->to_ne_bytes()));
    }

    static constexpr Self from_le_bytes(const ByteArray<sizeof(T)> &bytes) {
        return Self::from_ne_bytes(IS_LE ? bytes : reverse_bytes(bytes));
    }

    static constexpr Self from_le(const Self &source) {
        return Self(IS_LE ? source : source.reverse_bytes());
    }

    constexpr ByteArray<sizeof(T)> to_le_bytes() const {
        return IS_LE ? this->to_ne_bytes() : reverse_bytes(this->to_ne_bytes());
    }

    constexpr Self to_le() const {
        return Self(IS_LE ? *this : this->reverse_bytes());
    }

    static constexpr Self from_be_bytes(const ByteArray<sizeof(T)> &bytes) {
        return Self::from_ne_bytes(IS_BE ? bytes : reverse_bytes(bytes));
    }

    static constexpr Self from_be(const Self &source) {
        return Self(IS_BE ? source : source.reverse_bytes());
    }

    constexpr ByteArray<sizeof(T)> to_be_bytes() const {
        return IS_BE ? this->to_ne_bytes() : reverse_bytes(this->to_ne_bytes());
    }

    constexpr Self to_be() const {
        return Self(IS_BE ? *this : this->reverse_bytes());
    }

    // byte convert end =======================================================

    // arithmetic begin =======================================================

    constexpr Self operator+() const { return Self(*this); }
    constexpr Self operator-() const { return Self(-this->value); }

    // TODO(other arithmetic operators)

    // arithmetic end =========================================================
};

// TODO(cast low precision to high precision)
// TODO(prevent equality check between f32 and f64)
template <typename T1, typename T2>
inline constexpr bool operator==(const Number<T1> &l, const Number<T2> &r) {
    return static_cast<T2>(l.value) == r.value;
}

template <typename T1, typename T2>
inline constexpr bool operator!=(const Number<T1> &l, const Number<T2> &r) {
    return !(l == r);
}

template <typename T>
::std::ostream &operator<<(::std::ostream &s, const Number<T> &number) {
    return s << +number.value;
}

// TODO(i128/u128)

#define GENERAL_INT_IMPL(ALIAS, ORIGIN)                                        \
    using ALIAS = Number<ORIGIN>;                                              \
    namespace literal {                                                        \
    constexpr ALIAS operator""_##ALIAS(unsigned long long int val) {           \
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
    constexpr f##BITS operator""_f##BITS(long double val) {                    \
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
    ::std::size_t
    operator()(const ::rusty::numeric_types::Number<T> &number) const noexcept {
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
