//
// Copyright (c) 2025 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/cppalliance/buffers
//

#ifndef BOOST_BUFFERS_PREFIX_HPP
#define BOOST_BUFFERS_PREFIX_HPP

#include <boost/buffers/detail/config.hpp>
#include <boost/buffers/detail/type_traits.hpp>
#include <type_traits>

namespace boost {
namespace buffers {

/** Return the first n bytes of a buffer sequence
*/
/**@{*/
template<class>
void prefix(...) = delete;

// span-like types
template<
    template<class, std::size_t> class Span,
    class T, std::size_t Extent>
constexpr
auto
prefix(
    Span<T, Extent> const& bs,
    std::size_t n) ->
        typename std::enable_if<
            detail::is_span<Span<T, Extent>>::value
                && ! detail::has_prefix<T>::value,
            Span<T, Extent> >::type
{
    if(n <= bs.size())
        return bs.subspan(0, n);
    return bs;
}

// user-defined types
template<
    class T, class = typename std::enable_if<
        detail::has_prefix<T>::value>::type>
auto
prefix(
    T const& bs,
    std::size_t n) ->
        decltype(bs.prefix(n))
{
    return bs.prefix(n);
}
/**@}*/

/** Alias for the type of a prefix of a buffer sequence.
*/
template<class T>
using prefix_type = decltype(prefix(
    std::declval<T const&>(), std::size_t{1}));

} // buffers
} // boost

#endif
