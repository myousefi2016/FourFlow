// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_MULTI_UTIL_FOR_EACH_RANGE_HPP
#define BOOST_GEOMETRY_MULTI_UTIL_FOR_EACH_RANGE_HPP


#include <boost/geometry/util/for_each_range.hpp>
#include <boost/geometry/util/range_iterator_const_if_c.hpp>

#include <boost/geometry/multi/core/tags.hpp>


namespace boost { namespace geometry
{


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace for_each {


template <typename Multi, typename Actor, bool IsConst>
struct fe_range_multi
{
    static inline void apply(
                    typename add_const_if_c<IsConst, Multi>::type& multi,
                    Actor& actor)
    {
        for (typename range_iterator_const_if_c<IsConst, Multi>::type
                it = boost::begin(multi);
            it != boost::end(multi);
            ++it)
        {
            geometry::for_each_range(*it, actor);
        }
    }
};



}} // namespace detail::for_each
#endif // DOXYGEN_NO_DETAIL


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{


template <typename Geometry, typename Actor, bool IsConst>
struct for_each_range<multi_linestring_tag, true, Geometry, Actor, IsConst>
    : detail::for_each::fe_range_multi<Geometry, Actor, IsConst>
{};


template <typename Geometry, typename Actor, bool IsConst>
struct for_each_range<multi_polygon_tag, true, Geometry, Actor, IsConst>
    : detail::for_each::fe_range_multi<Geometry, Actor, IsConst>
{};


template <typename MultiPoint, typename Actor, bool IsConst>
struct for_each_range<multi_point_tag, true, MultiPoint, Actor, IsConst>
    : detail::for_each::fe_range_range<MultiPoint, Actor, IsConst>
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH



}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_MULTI_UTIL_FOR_EACH_RANGE_HPP
