// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_CORRECT_HPP
#define BOOST_GEOMETRY_ALGORITHMS_CORRECT_HPP

#include <cstddef>
#include <algorithm>
#include <functional>

#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/core/ring_type.hpp>
#include <boost/geometry/core/exterior_ring.hpp>
#include <boost/geometry/core/interior_rings.hpp>

#include <boost/geometry/geometries/concepts/check.hpp>

#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/disjoint.hpp>


namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace correct {


template <typename Box, std::size_t Dimension, std::size_t DimensionCount>
struct correct_box_loop
{
    typedef typename coordinate_type<Box>::type coordinate_type;

    static inline void apply(Box& box)
    {
        if (get<min_corner, Dimension>(box) > get<max_corner, Dimension>(box))
        {
            // Swap the coordinates
            coordinate_type max_value = get<min_corner, Dimension>(box);
            coordinate_type min_value = get<max_corner, Dimension>(box);
            set<min_corner, Dimension>(box, min_value);
            set<max_corner, Dimension>(box, max_value);
        }

        correct_box_loop
            <
                Box, Dimension + 1, DimensionCount
            >::apply(box);
    }
};



template <typename Box, std::size_t DimensionCount>
struct correct_box_loop<Box, DimensionCount, DimensionCount>
{
    static inline void apply(Box& box)
    {}

};


// correct an box: make min/max are correct
template <typename Box>
struct correct_box
{

    static inline void apply(Box& box)
    {
        // Currently only for Cartesian coordinates
        // TODO: adapt using strategies
        correct_box_loop
            <
                Box, 0, dimension<Box>::type::value
            >::apply(box);
    }
};


// close a linear_ring, if not closed
template <typename Ring, typename Predicate>
struct correct_ring
{
    typedef typename point_type<Ring>::type point_type;

    typedef typename strategy_area
        <
            typename cs_tag<point_type>::type,
            point_type
        >::type strategy_type;

    typedef detail::area::ring_area
            <
                Ring,
                geometry::point_order<Ring>::value,
                strategy_type
            > ring_area_type;


    static inline void apply(Ring& r)
    {
        // Check close-ness
        if (boost::size(r) > 2)
        {
            // check if closed, if not, close it
            if (geometry::disjoint(*boost::begin(r), *(boost::end(r) - 1)))
            {
                point_type first;
                geometry::copy_coordinates(*boost::begin(r), first);
                *(std::back_inserter(r)++) = first;
            }
        }
        // Check area
        Predicate predicate;
        if (predicate(ring_area_type::apply(r, strategy_type()), 0))
        {
            std::reverse(boost::begin(r), boost::end(r));
        }
    }
};

// correct a polygon: normalizes all rings, sets outer linear_ring clockwise, sets all
// inner rings counter clockwise
template <typename Polygon>
struct correct_polygon
{
    typedef typename ring_type<Polygon>::type ring_type;

    static inline void apply(Polygon& poly)
    {
        correct_ring<ring_type, std::less<double> >::apply(exterior_ring(poly));

        typedef typename boost::range_iterator
            <
                typename interior_type<Polygon>::type
            >::type iterator_type;

        for (iterator_type it = boost::begin(interior_rings(poly));
             it != boost::end(interior_rings(poly)); ++it)
        {
            correct_ring<ring_type, std::greater<double> >::apply(*it);
        }
    }
};


}} // namespace detail::correct
#endif // DOXYGEN_NO_DETAIL

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename Tag, typename Geometry>
struct correct {};

template <typename Box>
struct correct<box_tag, Box>
    : detail::correct::correct_box<Box>
{};

template <typename Ring>
struct correct<ring_tag, Ring>
    : detail::correct::correct_ring<Ring, std::less<double> >
{};

template <typename Polygon>
struct correct<polygon_tag, Polygon>
    : detail::correct::correct_polygon<Polygon>
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH

template <typename Geometry>
inline void correct(Geometry& geometry)
{
    concept::check<const Geometry>();

    dispatch::correct<typename tag<Geometry>::type, Geometry>::apply(geometry);
}

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_ALGORITHMS_CORRECT_HPP
