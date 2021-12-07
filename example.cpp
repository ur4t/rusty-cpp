// SPDX-License-Identifier: MIT
// Copyright(c) 2021 ur4t

#include <cassert>
#include <iomanip>
#include <iostream>

#define RUSTY_NO_DEFAULT_USING

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
    dbg(-0x12345678_i32, << ::std::showpos);
    dbg(+0x12345678_i32, << ::std::showpos);
    dbg(-0x80000000_i32, << ::std::showpos); // gcc warning: integer overflow
    dbg(+0x80000000_i32, << ::std::showpos);
    dbg(-0x87654321_i32, << ::std::showpos);
    dbg(-0x12345678_i32, << ::std::showpos);
}

void test_floating_literals() {

    using namespace ::rusty::numeric_types::literal;

    banner("testing floating literals");

    dbg(0.1_f32, << ::std::fixed << ::std::setprecision(60));
    dbg(0.1_f64, << ::std::fixed << ::std::setprecision(60));
    dbg(0.0_f32 == 0.0_f64, << ::std::boolalpha);
    dbg(0.1_f32 == 0.1_f64, << ::std::boolalpha);
}

void test_collections() {

    using namespace ::rusty::numeric_types;
    using namespace ::rusty::collections::ostream;
    using namespace ::rusty::collections;

    using ::rusty::collections::HashMap;
    using ::rusty::collections::Pair;
    using ::std::string;

    banner("testing collection ostream");

    auto pair_pair = Pair<Pair<i8, string>,
                          Pair<u8, Pair<Pair<i8, string>, Pair<u8, string>>>>{
        {-1, "-1"}, {2, {{-1, "-1"}, {2, "-2"}}}};
    dbg(pair_pair);
    auto vec = Vec<i32>{1, 2, 3};
    dbg(vec);
    auto arr = Array<string, 4>{"11", "22", "33", "44"};
    dbg(arr);
    auto map = HashMap<i32, string>{{1, "-1"}, {2, "-2"}};
    dbg(map);
}

void test_byte_convert() {

    using namespace ::rusty::numeric_types;
    using namespace ::rusty::numeric_types::literal;
    using namespace ::rusty::collections::ostream;
    using ::rusty::collections::Array;

    banner("testing byte convert");

    Array<u8, 4> arr = {1, 2, 3, 4};
    dbg(arr, << ::std::showbase << ::std::hex);
    dbg(u32::from_ne_bytes(arr), << ::std::showbase << ::std::hex);
    dbg(u64::from_ne_bytes((19880124.0_f64).to_ne_bytes()),
        << ::std::showbase << ::std::hex);
    dbg((0x12345678_u32).reverse_bytes(), << ::std::showbase << ::std::hex);
    dbg((1.1_f32).to_ne_bytes(), << ::std::showbase << ::std::hex);

    // // the following two cases are from
    // // https://en.cppreference.com/w/cpp/numeric/bit_cast

    auto f64v1 = 19880124.0_f64;
    auto u64v1 = bit_cast<u64>(f64v1);
    assert_eq(bit_cast<f64>(u64v1), f64v1); // round-trip
    dbg(f64v1);
    dbg(u64v1, << ::std::showbase << ::std::hex);

    auto u64v2 = 0x3fe9000000000000_u64;
    auto f64v2 = bit_cast<f64>(u64v2);
    assert_eq(bit_cast<u64>(f64v2), u64v2); // round-trip
    dbg(f64v2);
    dbg(u64v2, << ::std::showbase << ::std::hex);
    dbg(f64v2.to_be_bytes(), << ::std::showbase << ::std::hex);
    dbg(u64v2.to_ne_bytes(), << ::std::showbase << ::std::hex);
}

int main() {
    compiler_info();
    test_numeric_type_length();
    test_integer_literals();
    test_floating_literals();
    test_collections();
    test_byte_convert();
}
