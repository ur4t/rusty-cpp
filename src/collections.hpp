// SPDX-License-Identifier: MIT
// Copyright(c) 2021 ur4t

#ifndef __RUSTY_COLLECTIONS_HPP__
#define __RUSTY_COLLECTIONS_HPP__

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

#endif // __RUSTY_COLLECTIONS_HPP__
