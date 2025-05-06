//
// Copyright (c) 2023 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/cppalliance/buffers
//

#ifndef BOOST_BUFFERS_SANS_PREFIX_HPP
#define BOOST_BUFFERS_SANS_PREFIX_HPP

#include <boost/buffers/detail/config.hpp>
#include <boost/buffers/size.hpp>
#include <boost/buffers/suffix.hpp>

namespace boost {
namespace buffers {

namespace detail {

struct sans_prefix_impl
{
    template<class BufferSequence>
    suffix_type<BufferSequence>
    operator()(
        BufferSequence const& b,
        std::size_t n) const
    {
        static_assert(
            is_const_buffer_sequence<
                BufferSequence>::value,
            "Type requirements not met");

        auto const n0 = size(b);
        if(n < n0)
            return suffix(b, n0 - n);
        return suffix(b, 0);
    }
};

} // detail

/** Return a suffix of the buffer sequence.
*/
constexpr detail::sans_prefix_impl sans_prefix{};

} // buffers
} // boost

#endif
