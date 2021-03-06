// SPDX-License-Identifier: MIT
// Copyright(c) 2021 ur4t

#include <cassert>
#include <iomanip>
#include <iostream>

#include "rusty.hpp"

#define banner(x) ::std::cerr << "\n----------" << (x) << "--------------\n"

void compiler_info() {
    banner("compiler info");
#ifdef _MSC_VER
    dbg(_MSC_VER);
#else
    dbg(__VERSION__);
#endif
    dbg(__cplusplus);
}

void test_numeric_type_length() {
    using namespace ::rusty::numeric_types;
    banner("testing numeric type length");
    dbg(sizeof(i8));
    dbg(sizeof(i16));
    dbg(sizeof(i32));
    dbg(sizeof(i64));
    dbg(sizeof(isize));
    dbg(sizeof(u8));
    dbg(sizeof(u16));
    dbg(sizeof(u32));
    dbg(sizeof(u64));
    dbg(sizeof(usize));
    dbg(sizeof(f32));
    dbg(sizeof(f64));
}

void test_integer_literals() {
    using namespace ::rusty::numeric_types::literal;
    banner("testing integer literals");
    dbg(-0x123456789abcdef0_i64, << ::std::showpos);
    dbg(+0x123456789abcdef0_i64, << ::std::showpos);
    dbg(-0xfedcba9876543210_i64, << ::std::showpos);
    dbg(+0xfedcba9876543210_i64, << ::std::showpos);
    dbg(-0x8000000000000000_i64, << ::std::showpos);
    dbg(+0x8000000000000000_i64, << ::std::showpos);
    dbg(-0x12345678_i32, << ::std::showpos);
    dbg(+0x12345678_i32, << ::std::showpos);
    dbg(-0x87654321_i32, << ::std::showpos);
    dbg(+0x87654321_i32, << ::std::showpos);
    dbg(-0x80000000_i32, << ::std::showpos);
    dbg(+0x80000000_i32, << ::std::showpos);
}

void test_floating_literals() {
    using namespace ::rusty::numeric_types::literal;
    using namespace ::rusty::collections::ostream;
    using ::rusty::numeric_types::f32;
    using ::rusty::numeric_types::f64;
    banner("testing floating literals");
    dbg(0.1_f32, << ::std::fixed << ::std::setprecision(60));
    dbg(0.1_f64, << ::std::fixed << ::std::setprecision(60));
    dbg(f64(0.1_f32), << ::std::fixed << ::std::setprecision(60));
    dbg(f32(0.1_f64), << ::std::fixed << ::std::setprecision(60));
    dbg((f64(0.0_f32) == 0.0_f64), << ::std::boolalpha);
    dbg((f64(0.1_f32) == 0.1_f64), << ::std::boolalpha);
    dbg(f32::from_ne_bytes({0xcd, 0xcc, 0x8c, 0x3f}));
    dbg(f32::from_le_bytes({0xcd, 0xcc, 0x8c, 0x3f}));
    dbg(f32::from_be_bytes({0xcd, 0xcc, 0x8c, 0x3f}));
    dbg(f32::from_le(1.1_f32));
    dbg(f32::from_be(1.1_f32));
    dbg((1.1_f32).to_ne_bytes(), << ::std::showbase << ::std::hex);
    dbg((1.1_f32).to_le_bytes(), << ::std::showbase << ::std::hex);
    dbg((1.1_f32).to_be_bytes(), << ::std::showbase << ::std::hex);
    dbg((1.1_f32).to_le());
    dbg((1.1_f32).to_be());
    dbg((1.1_f32).reverse_bytes());
    dbg(f64::from_ne_bytes({0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f}));
    dbg(f64::from_le_bytes({0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f}));
    dbg(f64::from_be_bytes({0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f}));
    dbg(f64::from_le(1.1_f64));
    dbg(f64::from_be(1.1_f64));
    dbg((1.1_f64).to_ne_bytes(), << ::std::showbase << ::std::hex);
    dbg((1.1_f64).to_le_bytes(), << ::std::showbase << ::std::hex);
    dbg((1.1_f64).to_be_bytes(), << ::std::showbase << ::std::hex);
    dbg((1.1_f64).to_le());
    dbg((1.1_f64).to_be());
    dbg((1.1_f64).reverse_bytes());
}

void test_collections() {
    using namespace ::rusty::numeric_types;
    using namespace ::rusty::collections::ostream;
    using namespace ::rusty::collections;
    using ::std::string;
    banner("testing collection ostream");
    dbg((Pair<Pair<i8, string>, Pair<u8, Pair<u16, f64>>>{{128, "str"},
                                                          {256, {256, 0.8}}}));
    dbg((Vec<i32>{1, 2, 3}));
    dbg((Array<string, 4>{"11", "22", "33", "44"}));
    dbg((HashMap<i32, string>{{1, "str1"}, {2, "str2"}}));
}

void test_operators() {
    using namespace ::rusty::numeric_types;
    using namespace ::rusty::numeric_types::literal;
    banner("testing operators");
    dbg(+1_u64, << ::std::showbase << std::hex);
    dbg(-1_u64, << ::std::showbase << std::hex);
    dbg(~1_u64, << ::std::showbase << std::hex);
    dbg(!1_u64, << ::std::showbase << std::hex);
    dbg(+0_u64, << ::std::showbase << std::hex);
    dbg(-0_u64, << ::std::showbase << std::hex);
    dbg(~0_u64, << ::std::showbase << std::hex);
    dbg(!0_u64, << ::std::showbase << std::hex);
    auto u64_5 = 5_u64;
    dbg(u64_5);
    dbg(++u64_5);
    dbg(u64_5);
    dbg(u64_5++);
    dbg(u64_5);
    dbg(u64_5 += 3);
    dbg(u64_5 -= 3);
    dbg(u64_5 *= 3);
    dbg(u64_5 /= 3);
    dbg(u64_5 %= 3);
    dbg(u64_5 ^= 3);
    dbg(u64_5 &= 3);
    dbg(u64_5 |= 3);
    dbg(u64_5 <<= 3);
    dbg(u64_5 >>= 3);
    dbg(1_u64 + 2_u64 * 3_u64 - 4_u64 / 2_u64);
    dbg(100_u64 / 7_u64);
    dbg(100_u64 % 7_u64);
    dbg(f64(100_u64) / 7.0_f64);
    dbg(0.7_f64 * 142.857_f64);
    dbg(0.7_f64 * 142.8571_f64);
    dbg(0xaa5555aa_i32 ^ 0x01010101, << ::std::showbase << std::hex);
    dbg(0xaa5555aa_i32 & 0x01010101, << ::std::showbase << std::hex);
    dbg(0xaa5555aa_i32 | 0x01010101, << ::std::showbase << std::hex);
    dbg(0x123_u64 << 2, << ::std::showbase << std::hex);
    dbg(0x123_u64 >> 2, << ::std::showbase << std::hex);
    dbg(0x123_u64 && 0, << ::std::showbase << std::hex);
    dbg(0x123_u64 || 0, << ::std::showbase << std::hex);
    dbg(-0x80000000_i32 == +0x80000000_i32, << ::std::boolalpha);
    dbg(-0x80000000_i32 != +0x80000000_i32, << ::std::boolalpha);
    dbg(f64(0.1_f32) > 0.1_f64, << ::std::boolalpha);
    dbg(f64(0.1_f32) >= 0.1_f64, << ::std::boolalpha);
    dbg(f64(0.1_f32) < 0.1_f64, << ::std::boolalpha);
    dbg(f64(0.1_f32) <= 0.1_f64, << ::std::boolalpha);
}

int main() {
    compiler_info();
    // test_numeric_type_length();
    test_integer_literals();
    test_floating_literals();
    test_collections();
    test_operators();
}
