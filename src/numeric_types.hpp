// SPDX-License-Identifier: MIT
// Copyright(c) 2021 ur4t

#ifndef __RUSTY_NUMERIC_TYPES_HPP__
#define __RUSTY_NUMERIC_TYPES_HPP__

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

    // Implicit conversion from raw numeric literals
    // is not recommended, though allowed for convenience.
    // NOLINTNEXTLINE(google-explicit-constructor)
    constexpr Number<T>(const T &value) : value(value){};
    constexpr Number<T>() = default;
    constexpr Number<T>(const Self &) = default;
    // Disable implicit conversion between Number<> for safety.
    template <typename T2>
    constexpr explicit Number<T>(const Number<T2> &number)
        : value(static_cast<T>(number.value)){};

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

    static constexpr Self from_ne_bytes(const ByteArray<sizeof(T)> &bytes) {
        return Self{bit_cast<T>(bytes)};
    }

    constexpr ByteArray<sizeof(T)> to_ne_bytes() const & {
        return bit_cast<ByteArray<sizeof(T)>>(value);
    }

    // Based on https://en.cppreference.com/w/cpp/container/array/rbegin
    template <::std::size_t N>
    static constexpr ByteArray<N> reverse_bytes(ByteArray<N> bytes) {
        ByteArray<N> reversed_bytes;
        ::std::copy(bytes.crbegin(), bytes.crend(), reversed_bytes.begin());
        return reversed_bytes;
    }

    constexpr Self reverse_bytes() const & {
        return Self::from_ne_bytes(reverse_bytes(this->to_ne_bytes()));
    }

#define BYTE_CONVERT_IMPL(e)                                                   \
    static constexpr Self from_##e##_bytes(                                    \
        const ByteArray<sizeof(T)> &bytes) {                                   \
        return Self::from_ne_bytes(is_##e ? bytes : reverse_bytes(bytes));     \
    }                                                                          \
    static constexpr Self from_##e(const Self &source) {                       \
        return Self(is_##e ? source : source.reverse_bytes());                 \
    }                                                                          \
    constexpr ByteArray<sizeof(T)> to_##e##_bytes() const & {                  \
        return is_##e ? this->to_ne_bytes()                                    \
                      : reverse_bytes(this->to_ne_bytes());                    \
    }                                                                          \
    constexpr Self to_##e() const & {                                          \
        return Self(is_##e ? *this : this->reverse_bytes());                   \
    }

    BYTE_CONVERT_IMPL(le)
    BYTE_CONVERT_IMPL(be)

#undef BYTE_CONVERT_IMPL

    inline Self operator++() & { return Self(++this->value); }
    inline Self operator--() & { return Self(--this->value); }
    inline Self operator++(int) & { return Self(this->value++); } // NOLINT
    inline Self operator--(int) & { return Self(this->value--); } // NOLINT

#define UNARY_OP_IMPL(op)                                                      \
    inline constexpr Self operator op() const & { return Self(op this->value); }

    UNARY_OP_IMPL(+)
    UNARY_OP_IMPL(-)
    UNARY_OP_IMPL(~)
    UNARY_OP_IMPL(!)

#undef UNARY_OP_IMPL

#define BINARY_OP_IMPL(op)                                                     \
    inline constexpr Self operator op(const Self &oprand) const & {            \
        return Self(this->value op oprand.value);                              \
    }                                                                          \
    inline Self operator op##=(const Self &oprand) & {                         \
        return this->value op## = oprand.value;                                \
    }

    BINARY_OP_IMPL(+)
    BINARY_OP_IMPL(-)
    BINARY_OP_IMPL(*)
    BINARY_OP_IMPL(/)
    BINARY_OP_IMPL(%)
    BINARY_OP_IMPL(&)
    BINARY_OP_IMPL(|)
    BINARY_OP_IMPL(^)
    BINARY_OP_IMPL(<<)
    BINARY_OP_IMPL(>>)

    inline constexpr Self operator&&(const Self &oprand) const & {
        return Self(this->value && oprand.value);
    }

    inline constexpr Self operator||(const Self &oprand) const & {
        return Self(this->value || oprand.value);
    }

#undef BINARY_OP_IMPL

#define COMPARISON_OP_IMPL(op)                                                 \
    friend inline constexpr bool operator op(const Self &l, const Self &r) {   \
        return l.value op r.value;                                             \
    }

    COMPARISON_OP_IMPL(==)
    COMPARISON_OP_IMPL(!=)
    COMPARISON_OP_IMPL(<)
    COMPARISON_OP_IMPL(>)
    COMPARISON_OP_IMPL(<=)
    COMPARISON_OP_IMPL(>=)

#undef COMPARISON_OP_IMPL

    friend ::std::ostream &operator<<(::std::ostream &s,
                                      const Number<T> &number) {
        return s << +number.value;
    }

    friend ::std::istream &operator>>(::std::istream &s, Number<T> &number) {
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
    ::std::size_t operator()(
        const ::rusty::numeric_types::Number<T> &number) const &noexcept {
        return ::std::hash<T>{}(number.value);
    }
};

#endif // __RUSTY_NUMERIC_TYPES_HPP__
