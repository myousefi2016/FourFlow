// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ITERATORS_RANGE_TYPE_HPP
#define BOOST_GEOMETRY_ITERATORS_RANGE_TYPE_HPP

#include <boost/type_traits.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <boost/geometry/core/ring_type.hpp>
#include <boost/geometry/core/tag.hpp>
#include <boost/geometry/core/tags.hpp>


namespace boost { namespace geometry {


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{


template <typename GeometryTag, typename Geometry>
struct range_type
{
    // Even if it is not recognized, define itself as a type.
    // This enables calling range_type over any range
    // (not necessarily a geometry)

    // Furthermore, applicable for ring/linestring
    typedef Geometry type;
};


template <typename Geometry>
struct range_type<point_tag, Geometry>
{
    typedef void type;
};


template <typename Geometry>
struct range_type<polygon_tag, Geometry>
{
    typedef typename ring_type<Geometry>::type type;
};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH



/*!
\brief Meta-function defining a type which is a boost-range.
\details
- For linestrings and rings, it defines the type itself.
- For polygons it defines the ring type.
- For multi-points, it defines the type itself
- For multi-polygons and multi-linestrings, it defines the single-version
    (so in the end the linestring and ring-type-of-multi-polygon)
\ingroup iterators
*/
template <typename Geometry>
struct range_type
{
    typedef typename dispatch::range_type
        <
            typename tag<Geometry>::type,
            Geometry
        >::type type;
};


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_ITERATORS_RANGE_TYPE_HPP
