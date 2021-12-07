// SPDX-License-Identifier: MIT
// Copyright(c) 2021 ur4t

#ifndef __RUSTY_MACROS_HPP__
#define __RUSTY_MACROS_HPP__

#define dbg(x, ...)                                                            \
    [&] {                                                                      \
        auto flag = ::std::cerr.flags();                                       \
        ::std::cerr << "[" __FILE__ ":" << __LINE__                            \
                    << "] " #x " = " __VA_ARGS__ << (x) << ::std::endl;        \
        ::std::cerr.flags(flag);                                               \
        return x;                                                              \
    }()

#define assert_eq(left, right) assert((left) == (right))
#define assert_ne(left, right) assert(!((left) == (right)))

#endif // __RUSTY_MACROS_HPP__
