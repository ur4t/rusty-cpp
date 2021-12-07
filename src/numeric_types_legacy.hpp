// SPDX-License-Identifier: MIT
// Copyright(c) 2021 ur4t

#ifndef __NUMERIC_TYPES_LEFACY_HPP__
#define __NUMERIC_TYPES_LEFACY_HPP__

#include <cstdint>
#include <limits>

namespace rusty::numeric_types_legacy {

// numeric user-defined literals cannot capture the sign
// static_cast<i32>(0x80000000) is -0x80000000
// so -0x80000000_i32 is -(0x80000000_i32)
// which makes gcc throw overflow warning

// compile time range check is not implemented.
// i128/u128 is not implemented.

#define GENERAL_INT_IMPL(ALIAS, ORIGIN)                                        \
    using ALIAS = ORIGIN;                                                      \
    namespace literal {                                                        \
    constexpr ALIAS operator""_##ALIAS(unsigned long long int val) {           \
        return static_cast<ALIAS>(val);                                        \
    }                                                                          \
    } // namespace literal

#define FIXED_INT_IMPL(BITS)                                                   \
    GENERAL_INT_IMPL(i##BITS, int##BITS##_t)                                   \
    GENERAL_INT_IMPL(u##BITS, uint##BITS##_t)

#define FLOATING_IMPL(BITS, ORIGIN)                                            \
    using f##BITS = ORIGIN;                                                    \
    static_assert(::std::numeric_limits<f##BITS>::is_iec559 &&                 \
                      sizeof(f##BITS) == (BITS) / 8,                           \
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

} // namespace rusty::numeric_types_legacy

#endif // __NUMERIC_TYPES_LEFACY_HPP__
