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

#endif // __RUSTY_COLLECTIONS_HPP__
