//
// Copyright (c) 2025 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/cppalliance/buffers
//

#ifndef BOOST_BUFFERS_SUFFIX_HPP
#define BOOST_BUFFERS_SUFFIX_HPP

#include <boost/buffers/detail/config.hpp>
#include <boost/buffers/detail/type_traits.hpp>
#include <type_traits>

namespace boost {
namespace buffers {

/** Return the last n bytes of a buffer sequence
*/
/**@{*/
template<class>
void
suffix(...) = delete;

// Span-like types
template<
    template<class, std::size_t> class Span,
    class T, std::size_t Extent>
constexpr
auto
suffix(
    Span<T, Extent> const& b,
    std::size_t n) ->
        typename std::enable_if<
            detail::is_span<Span<T, Extent>>::value
                && ! detail::has_suffix<T>::value,
            Span<T, Extent> >::type
{
    if(n < b.size())
        return b.subspan(b.size() - n);
    return b;
}

// User defined types
template<
    class T, class = typename std::enable_if<
        detail::has_suffix<T>::value>::type>
auto
suffix(
    T const& bs,
    std::size_t n) ->
        decltype(bs.suffix(n))
{
    return bs.suffix(n);
}
/**@}*/

/** Alias for the type of a suffix of a buffer sequence.
*/
template<class T>
using suffix_type = decltype(suffix(
    std::declval<T const&>(), std::size_t{1}));

} // buffers
} // boost

#endif
