//
// Copyright (c) 2023 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/cppalliance/buffers
//

#ifndef BOOST_BUFFERS_ALGORITHM_HPP
#define BOOST_BUFFERS_ALGORITHM_HPP

#include <boost/buffers/detail/config.hpp>
#include <boost/buffers/buffer_size.hpp>
#include <boost/buffers/const_buffer.hpp>
#include <boost/buffers/mutable_buffer.hpp>
#include <boost/buffers/range.hpp>
#include <boost/buffers/tag_invoke.hpp>
#include <boost/buffers/type_traits.hpp>

namespace boost {
namespace buffers {

template<class BufferSequence>
void
tag_invoke(
    prefix_tag const&,
    BufferSequence const&,
    std::size_t) = delete;

template<class BufferSequence>
void
tag_invoke(
    suffix_tag const&,
    BufferSequence const&,
    std::size_t) = delete;

/** Returns the type of a prefix of a buffer sequence.
*/
template<class BufferSequence>
using prefix_type = decltype(
    tag_invoke(
        prefix_tag{},
        std::declval<BufferSequence const&>(),
        std::size_t{}));

/** Returns the type of a suffix of a buffer sequence.
*/
template<class BufferSequence>
using suffix_type = decltype(
    tag_invoke(
        suffix_tag{},
        std::declval<BufferSequence const&>(),
        std::size_t{}));

namespace detail {

struct prefix_impl
{
    template<class BufferSequence>
    prefix_type<BufferSequence>
    operator()(
        BufferSequence const& b,
        std::size_t n) const
    {
        static_assert(
            is_const_buffer_sequence<
                BufferSequence>::value,
            "Type requirements not met");

        return tag_invoke(
            prefix_tag{}, b, n);
    }
};

struct sans_suffix_impl
{
    template<class BufferSequence>
    prefix_type<BufferSequence>
    operator()(
        BufferSequence const& b,
        std::size_t n) const
    {
        auto const n0 = buffer_size(b);
        if(n < n0)
            return tag_invoke(
                prefix_tag{}, b, n0 - n);
        return tag_invoke(
            prefix_tag{}, b, 0);
    }
};

struct suffix_impl
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

        return tag_invoke(
            suffix_tag{}, b, n);
    }
};

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

        auto const n0 = buffer_size(b);
        if(n < n0)
            return tag_invoke(
                suffix_tag{}, b, n0 - n);
        return tag_invoke(
            suffix_tag{}, b, 0);
    }
};

struct front_impl
{
    template<
        class MutableBufferSequence
        , class = typename std::enable_if<
            is_mutable_buffer_sequence<
                MutableBufferSequence>::value
        >::type>
    mutable_buffer
    operator()(
        MutableBufferSequence const& bs) const noexcept
    {
        auto const it = begin(bs);
        if(it != end(bs))
            return *it;
        return {};
    }

    template<
        class ConstBufferSequence
        , class = typename std::enable_if<
            ! is_mutable_buffer_sequence<
                ConstBufferSequence>::value
        >::type>
    const_buffer
    operator()(
        ConstBufferSequence const& bs) const noexcept
    {
        static_assert(
            is_const_buffer_sequence<
                ConstBufferSequence>::value,
            "Type requirements not met");

        auto const it = bs.begin();
        if(it != bs.end())
            return *it;
        return {};
    }
};

} // detail

/** Return a prefix of the buffer sequence.
*/
constexpr detail::prefix_impl prefix{};

/** Return a suffix of the buffer sequence.
*/
constexpr detail::suffix_impl suffix{};

/** Return a suffix of the buffer sequence.
*/
constexpr detail::sans_prefix_impl sans_prefix{};

/** Return a prefix of the buffer sequence.
*/
constexpr detail::sans_suffix_impl sans_suffix{};

/** Return the first buffer in a sequence.
*/
constexpr detail::front_impl front{};

} // buffers
} // boost

#endif
